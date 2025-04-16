#pragma once

#include <mutex>
#include <thread>
#include <unordered_map>
#include <condition_variable>
#include <unordered_set>
#include "chunkcoord.h"
#include "chunk.h"
#include <queue>
#include <deque>

class World;

class ChunkManager
{
public:

	ChunkManager(World* world);

	void loadChunk(ChunkCoord coord);
	void unloadChunk(ChunkCoord coord);
	void update();
	void remeshChunk(ChunkCoord coord);

	const std::unordered_map<ChunkCoord, Chunk*>& getLoadedChunks();
	Chunk* getLoadedChunk(ChunkCoord coordinate);
	std::recursive_mutex& getChunkMutex();
private:
	World* world;

	std::unordered_map<ChunkCoord, Chunk*> loadedChunks;

	std::queue<ChunkCoord> chunksToGenerate;
	std::deque<Chunk*> chunksToMesh;
	std::unordered_set<Chunk*> chunksToUnload;

	std::mutex meshMutex;
	std::mutex genMutex;
	std::recursive_mutex chunksMutex;

	std::thread generationThread;
	std::thread meshingThread;

	std::condition_variable genCondition;
	std::condition_variable meshCondition;

	void init();
	void genWorker();
	void meshWorker();
};
