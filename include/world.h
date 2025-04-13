#pragma once

#include "chunk.h"
#include <unordered_map>
#include "shader.h"
#include <FastNoiseSIMD.h>
#include <queue>
#include "player.h"
#include <mutex>

constexpr int RENDER_DISTANCE = 5;
constexpr int RENDER_HEIGHT = 1;

constexpr int chunkLoadDirections[6][3] = { { 1, 0, 0 }, { 0, 0, 1 }, { -1, 0, 0, }, { 0, 0, -1 }, { 0, 1, 0 }, { 0, -1, 0 } };

class World
{
public:

	World(const Shader& terrainShader);

	void updateLoadedChunks(ChunkCoord& newCoordinate);
	void loadChunk(ChunkCoord coordinate);

	void updateWorld(Player& player);
	void renderWorld();

	BlockType getBlockAt(int x, int y, int z);
	Chunk* getChunkByCoordinate(ChunkCoord coord);
private:
	BlockData blockData;
	std::unordered_map<ChunkCoord, Chunk*> loadedChunks;

	std::queue<ChunkCoord> chunksToUpdate;
	std::recursive_mutex chunksMutex;
	ChunkCoord lastplayerCoord;

	unsigned int shaderModelLoc;
	FastNoiseSIMD* noise;
};
