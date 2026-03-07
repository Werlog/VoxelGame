#include "chunkmanager.h"
#include "world.h"
#include "profiling/profiler.h"

ChunkManager::ChunkManager(World* world)
	: saveLoader(world), chunkGenerator(*world, this)
{
	this->world = world;

	init();
}

void ChunkManager::loadChunk(ChunkCoord coord)
{
	if (loadedChunks.find(coord) != loadedChunks.end()) return;

	auto it = savedChunks.find(coord);
	if (it != savedChunks.end())
	{
		loadedChunks.insert({ coord, it->second });
		remeshChunk(it->first);
		savedChunks.erase(it);
	}

	chunksToLoad.push_back(coord);
	loadCondition.notify_one();
}

void ChunkManager::unloadChunk(ChunkCoord coord)
{
	if (loadedChunks.find(coord) == loadedChunks.end())
		return;

	chunksToUnload.insert(coord);
}

void ChunkManager::update()
{
	PROFILER_ZONE;
	for (auto it = loadedChunks.begin(); it != loadedChunks.end(); it++)
	{
		std::shared_ptr<Chunk> chunk = it->second;
		if (chunk == nullptr)
			continue;

		std::shared_ptr<ChunkMesh> pendingMesh = std::atomic_exchange_explicit(&chunk->pendingMesh, std::shared_ptr<ChunkMesh>{}, std::memory_order_acquire);

		if (pendingMesh != nullptr)
		{
			chunk->gpuMesh = pendingMesh;
			chunk->createChunkMesh();
		}
	}

	int unloadedCount = 0;
	for (auto it = chunksToUnload.begin(); it != chunksToUnload.end();)
	{
		if (unloadedCount >= MAX_UNLOAD_COUNT)
			break;
		PROFILER_ZONE_N("Unload chunk");

		std::shared_ptr<Chunk> chunk = getLoadedChunk(*it);

		loadedChunks.erase(*it);
		if (chunk->wasModified())
		{
			savedChunks.insert({ chunk->getCoord(), chunk });
		}
		chunk->unloadMesh();

		it = chunksToUnload.erase(it);
		unloadedCount++;
	}

	while (!readyChunks.empty())
	{
		std::shared_ptr<Chunk> chunk = readyChunks.pop_front().value();
		
		loadedChunks.insert({ chunk->getCoord(), chunk });
	}
}

void ChunkManager::remeshChunk(ChunkCoord coord, bool pushToFront)
{
	std::shared_ptr<Chunk> chunk = getLoadedChunk(coord);

	if (chunk == nullptr)
		return;

	if (chunksToMesh.contains(chunk))
	{
		return;
	}
	if (pushToFront)
		chunksToMesh.push_front(chunk);
	else
		chunksToMesh.push_back(chunk);
	meshCondition.notify_one();
}

const std::unordered_map<ChunkCoord, std::shared_ptr<Chunk>>& ChunkManager::getLoadedChunks()
{
	return loadedChunks;
}

const std::unordered_map<ChunkCoord, std::shared_ptr<Chunk>>& ChunkManager::getSavedChunks()
{
	return savedChunks;
}

std::shared_ptr<Chunk> ChunkManager::getLoadedChunk(ChunkCoord coordinate)
{
	auto it = loadedChunks.find(coordinate);
	if (it == loadedChunks.end())
		return nullptr;

	return it->second;
}

std::shared_ptr<Chunk> ChunkManager::getSavedChunk(ChunkCoord coordinate)
{
	auto it = savedChunks.find(coordinate);
	if (it == loadedChunks.end())
		return nullptr;

	return it->second;
}

void ChunkManager::clearSavedChunks()
{
	savedChunks.clear();
}

void ChunkManager::init()
{
	std::thread genThread(&ChunkManager::loadWorker, this);
	std::thread meshThread(&ChunkManager::meshWorker, this);

	genThread.detach();
	meshThread.detach();
}

void ChunkManager::loadWorker()
{
	PROFILER_THREAD_NAME("Chunk Load Worker");
	while (true)
	{
		std::unique_lock lock(loadMutex);

		loadCondition.wait(lock, [this] { return !chunksToLoad.empty(); });

		lock.unlock();

		std::optional<ChunkCoord> loadCoord = chunksToLoad.pop_front();
		if (!loadCoord.has_value())
			continue;

		ChunkCoord coord = loadCoord.value();

		std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>(coord, world);

		bool loaded = saveLoader.loadChunk(chunk);
		if (!loaded)
		{
			chunkGenerator.setChunk(chunk);
			chunkGenerator.generate();
		}

		for (const auto& direction : worldDirections)
		{
			ChunkCoord nCoord = ChunkCoord{ coord.x + direction[0], coord.y + direction[1], coord.z + direction[2] };
			remeshChunk(nCoord);
		}
		
		remeshChunk(coord);

		readyChunks.push_back(chunk);
	}
}

void ChunkManager::meshWorker()
{
	PROFILER_THREAD_NAME("Chunk Meshing Worker");
	while (true)
	{
		std::unique_lock lock(meshMutex);

		meshCondition.wait(lock, [this] { return !chunksToMesh.empty(); });

		lock.unlock();

		std::optional<std::shared_ptr<Chunk>> chunkOpt = chunksToMesh.pop_front();
		if (!chunkOpt.has_value())
			continue;

		std::shared_ptr<Chunk> chunk = chunkOpt.value();

		if (chunk == nullptr)
			continue;

		std::shared_ptr<ChunkMesh> newMesh = std::make_shared<ChunkMesh>();

		chunk->updateLight(world->getBlockData());
		chunk->generateMesh(world->getBlockData(), newMesh);

		std::atomic_store_explicit(&chunk->pendingMesh, newMesh, std::memory_order_release);
	}
}
