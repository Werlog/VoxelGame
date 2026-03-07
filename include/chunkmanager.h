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
#include "saving/worldloader.h"
#include "generation/chunkgenerator.h"
#include "util/tsQueue.h"

class World;

struct BlockMod
{
	int x, y, z;
	BlockType block;
};

class ChunkManager
{
public:

	ChunkManager(World* world);

	void loadChunk(ChunkCoord coord);
	void unloadChunk(ChunkCoord coord);
	void update();
	void remeshChunk(ChunkCoord coord, bool pushToFront = false);

	const std::unordered_map<ChunkCoord, std::shared_ptr<Chunk>>& getLoadedChunks();
	const std::unordered_map<ChunkCoord, std::shared_ptr<Chunk>>& getSavedChunks();
	std::shared_ptr<Chunk> getLoadedChunk(ChunkCoord coordinate);
	std::shared_ptr<Chunk> getSavedChunk(ChunkCoord coordinate);

	void clearSavedChunks();
private:
	World* world;
	WorldLoader saveLoader;
	ChunkGenerator chunkGenerator;

	std::unordered_map<ChunkCoord, std::shared_ptr<Chunk>> loadedChunks;
	std::unordered_map<ChunkCoord, std::shared_ptr<Chunk>> savedChunks;

	ts_queue<ChunkCoord> chunksToLoad;
	ts_queue<std::shared_ptr<Chunk>> readyChunks; // Chunks that were just loaded by the loadWorker

	ts_queue<std::shared_ptr<Chunk>> chunksToMesh;
	std::unordered_set<ChunkCoord> chunksToUnload;

	std::mutex loadMutex;
	std::mutex meshMutex;

	std::condition_variable loadCondition;
	std::condition_variable meshCondition;

	void init();
	void loadWorker();
	void meshWorker();
};
