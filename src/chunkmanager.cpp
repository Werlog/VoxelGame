#include "chunkmanager.h"
#include "world.h"
#include "generation/chunkgenerator.h"

ChunkManager::ChunkManager(World* world)
	: saveLoader(world)
{
	this->world = world;

	init();
}

void ChunkManager::loadChunk(ChunkCoord coord)
{
	{
		std::lock_guard lock(chunksMutex);
		if (loadedChunks.find(coord) != loadedChunks.end()) return;

		auto it = savedChunks.find(coord);
		if (it != savedChunks.end())
		{
			loadedChunks.insert({ coord, it->second });
			remeshChunk(it->first);
			savedChunks.erase(it);
		}
	}

	std::lock_guard genLock(loadMutex);
	chunksToLoad.push(coord);
	loadCondition.notify_one();
}

void ChunkManager::unloadChunk(ChunkCoord coord)
{
	{
		std::lock_guard lock(chunksMutex);
		if (loadedChunks.find(coord) == loadedChunks.end())
			return;
	}

	chunksToUnload.insert(coord);
}

void ChunkManager::update()
{
	for (auto it = loadedChunks.begin(); it != loadedChunks.end(); it++)
	{
		std::shared_ptr<Chunk> chunk = it->second;
		if (chunk == nullptr)
			continue;

		if (chunk->shouldUpdateMesh.load())
		{
			chunk->createChunkMesh();
			chunk->shouldUpdateMesh.store(false);
		}
	}

	int unloadedCount = 0;
	for (auto it = chunksToUnload.begin(); it != chunksToUnload.end();)
	{
		if (unloadedCount >= MAX_UNLOAD_COUNT)
			break;

		std::shared_ptr<Chunk> chunk = getLoadedChunk(*it);

		{
			std::lock_guard lock(chunksMutex);
			loadedChunks.erase(*it);
			if (chunk->wasModified())
			{
				savedChunks.insert({ chunk->getCoord(), chunk });
			}
		}
		chunk->unloadMesh();

		it = chunksToUnload.erase(it);
		unloadedCount++;
	}
}

void ChunkManager::remeshChunk(ChunkCoord coord, bool pushToFront)
{
	std::shared_ptr<Chunk> chunk = getLoadedChunk(coord);

	if (chunk == nullptr)
		return;

	std::lock_guard lock(meshMutex);
	if (std::find(chunksToMesh.begin(), chunksToMesh.end(), chunk) != chunksToMesh.end())
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
	std::lock_guard lock(chunksMutex);
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

std::recursive_mutex& ChunkManager::getChunkMutex()
{
	return chunksMutex;
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
	while (true)
	{
		std::unique_lock lock(loadMutex);

		loadCondition.wait(lock, [this] { return !chunksToLoad.empty(); });

		ChunkCoord coord = chunksToLoad.front();
		chunksToLoad.pop();

		lock.unlock();

		std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>(coord, world);

		bool loaded = saveLoader.loadChunk(chunk);
		if (!loaded)
		{
			ChunkGenerator generator = ChunkGenerator(chunk, *world, this);
			generator.generate();
		}

		{
			std::lock_guard chunksLock(chunksMutex);
			loadedChunks.insert({ coord, chunk });
		}

		for (const auto& direction : worldDirections)
		{
			ChunkCoord nCoord = ChunkCoord{ coord.x + direction[0], coord.y + direction[1], coord.z + direction[2] };
			remeshChunk(nCoord);
		}
		
		remeshChunk(coord);
	}
}

void ChunkManager::meshWorker()
{
	while (true)
	{
		std::unique_lock lock(meshMutex);

		meshCondition.wait(lock, [this] { return !chunksToMesh.empty(); });

		std::shared_ptr<Chunk> chunk = chunksToMesh.front();
		chunksToMesh.pop_front();

		lock.unlock();

		if (chunk == nullptr)
		{
			continue;
		}

		chunk->updateLight(world->getBlockData());
		chunk->generateMesh(world->getBlockData());
		chunk->shouldUpdateMesh.store(true);
	}
}
