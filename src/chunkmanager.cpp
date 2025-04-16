#include "chunkmanager.h"
#include "world.h"

ChunkManager::ChunkManager(World* world)
{
	this->world = world;

	init();
}

void ChunkManager::loadChunk(ChunkCoord coord)
{
	{
		std::lock_guard chunkLock(chunksMutex);
		if (loadedChunks.find(coord) != loadedChunks.end()) return;
	}

	{
		std::lock_guard lock(genMutex);
		chunksToGenerate.push(coord);
	}
	
	genCondition.notify_one();
}

void ChunkManager::unloadChunk(ChunkCoord coord)
{
	Chunk* chunk = getLoadedChunk(coord);
	if (chunk == nullptr || chunksToUnload.find(chunk) != chunksToUnload.end())
		return;
	chunksToUnload.insert(chunk);
}

void ChunkManager::update()
{
	for (auto it = loadedChunks.begin(); it != loadedChunks.end(); it++)
	{
		Chunk* chunk = it->second;
		if (chunk == nullptr)
			continue;

		if (chunk->shouldUpdateMesh.load())
		{
			chunk->createChunkMesh();
			chunk->shouldUpdateMesh.store(false);
		}
	}

	for (auto it = chunksToUnload.begin(); it != chunksToUnload.end();)
	{
		Chunk* chunk = *it;
		
		if (!chunk->toBeRemeshed.load())
		{
			auto chunkIt = loadedChunks.find(chunk->getCoord());
			if (chunkIt != loadedChunks.end())
			{
				std::lock_guard chunkLock(chunksMutex);
				loadedChunks.erase(chunkIt);
			}
			delete chunk;

			it = chunksToUnload.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void ChunkManager::remeshChunk(ChunkCoord coord)
{
	Chunk* chunk = getLoadedChunk(coord);
	if (chunk == nullptr)
		return;

	if (chunksToUnload.find(chunk) != chunksToUnload.end())
	{
		return;
	}

	std::lock_guard lock(meshMutex);
	auto it = std::find(chunksToMesh.begin(), chunksToMesh.end(), chunk);
	if (it != chunksToMesh.end())
		chunksToMesh.erase(it);

	chunk->toBeRemeshed.store(true);
	chunksToMesh.push_back(chunk);
	meshCondition.notify_one();
}

const std::unordered_map<ChunkCoord, Chunk*>& ChunkManager::getLoadedChunks()
{
	return loadedChunks;
}

Chunk* ChunkManager::getLoadedChunk(ChunkCoord coordinate)
{
	std::lock_guard lock(chunksMutex);
	auto it = loadedChunks.find(coordinate);
	if (it == loadedChunks.end())
		return nullptr;

	return it->second;
}

std::recursive_mutex& ChunkManager::getChunkMutex()
{
	return chunksMutex;
}

void ChunkManager::init()
{
	std::thread genThread(&ChunkManager::genWorker, this);
	std::thread meshThread(&ChunkManager::meshWorker, this);

	genThread.detach();
	meshThread.detach();
}

void ChunkManager::genWorker()
{
	while (true)
	{
		std::unique_lock lock(genMutex);

		genCondition.wait(lock, [this] { return !chunksToGenerate.empty(); });

		ChunkCoord coord = chunksToGenerate.front();
		chunksToGenerate.pop();

		lock.unlock();

		Chunk* chunk = new Chunk(coord, world);
		chunk->generateChunk(world->getNoise());

		{
			std::lock_guard chunksLock(chunksMutex);
			loadedChunks.insert({ coord, chunk });
		}

		for (const auto& direction : chunkLoadDirections)
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

		Chunk* chunk = chunksToMesh.front();
		chunksToMesh.pop_front();

		lock.unlock();

		if (chunk == nullptr)
			continue;


		chunk->generateMesh(world->getBlockData());
		chunk->shouldUpdateMesh.store(true);
		chunk->toBeRemeshed.store(false);
	}
}
