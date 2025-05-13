#pragma once

#include "chunk.h"
#include <unordered_map>
#include "shader.h"
#include <FastNoiseSIMD.h>
#include <queue>
#include "player.h"
#include <mutex>
#include "chunkmanager.h"

constexpr int RENDER_DISTANCE = 10;
constexpr int RENDER_HEIGHT = 1;
constexpr int MAX_UNLOAD_COUNT = 2; // Maximum amount of chunks to unload per frame

constexpr int worldDirections[6][3] = { { 1, 0, 0 }, { 0, 0, 1 }, { -1, 0, 0, }, { 0, 0, -1 }, { 0, 1, 0 }, { 0, -1, 0 } };

class World
{
public:

	World(const Shader& terrainShader);

	void updateLoadedChunks(ChunkCoord& newCoordinate);
	void loadChunk(ChunkCoord coordinate);
	void remeshChunk(ChunkCoord coordinate);

	void updateWorld(Player& player);
	void renderWorld(const ChunkCoord& playerCoord, const glm::vec3& cameraPos);

	BlockType getBlockAt(int x, int y, int z);
	void setBlockAt(int x, int y, int z, BlockType newBlock);
	void modifyBlockAt(int x, int y, int z, BlockType newBlock);

	std::shared_ptr<Chunk> getChunkByCoordinate(ChunkCoord coord);
	FastNoiseSIMD* getNoise();
	FastNoiseSIMD* getTreeNoise();
	FastNoiseSIMD* getCaveNoise();
	int getWorldSeed();
	BlockData& getBlockData();
private:
	BlockData blockData;
	ChunkManager chunkManager;
	int seed;

	ChunkCoord lastplayerCoord;

	unsigned int shaderModelLoc;
	FastNoiseSIMD* noise;
	FastNoiseSIMD* treeDensityNoise;
	FastNoiseSIMD* caveNoise;

	void setupWorldGen();
};
