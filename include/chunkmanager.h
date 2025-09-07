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

	void addBlockMod(ChunkCoord coord, BlockMod mod);
	void resolveBlockMods(std::shared_ptr<Chunk> chunk);

	const std::unordered_map<ChunkCoord, std::shared_ptr<Chunk>>& getLoadedChunks();
	const std::unordered_map<ChunkCoord, std::shared_ptr<Chunk>>& getSavedChunks();
	std::shared_ptr<Chunk> getLoadedChunk(ChunkCoord coordinate);
	std::recursive_mutex& getChunkMutex();

	void clearSavedChunks();
private:
	World* world;
	WorldLoader saveLoader;

	std::unordered_map<ChunkCoord, std::shared_ptr<Chunk>> loadedChunks;
	std::unordered_map<ChunkCoord, std::shared_ptr<Chunk>> savedChunks;

	std::unordered_map<ChunkCoord, std::vector<BlockMod>> blockMods;

	std::queue<ChunkCoord> chunksToGenerate;
	std::deque<std::shared_ptr<Chunk>> chunksToMesh;
	std::unordered_set<ChunkCoord> chunksToUnload;

	std::mutex meshMutex;
	std::mutex genMutex;
	std::recursive_mutex chunksMutex;
	std::mutex blockModMutex;

	std::thread generationThread;
	std::thread meshingThread;

	std::condition_variable genCondition;
	std::condition_variable meshCondition;

	void init();
	void genWorker();
	void meshWorker();
};
