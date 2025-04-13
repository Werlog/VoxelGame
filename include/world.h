#pragma once

#include "chunk.h"
#include <unordered_map>
#include "shader.h"
#include <FastNoiseSIMD.h>

constexpr int WORLD_SIZE_X = 10;
constexpr int WORLD_SIZE_Y = 2;
constexpr int WORLD_SIZE_Z = 10;

class World
{
public:

	World(const Shader& terrainShader);

	void generateWorld();
	void renderWorld();

	BlockType getBlockAt(int x, int y, int z);
	Chunk* getChunkByCoordinate(ChunkCoord coord);
private:
	BlockData blockData;
	std::unordered_map<ChunkCoord, Chunk*> loadedChunks;
	unsigned int shaderModelLoc;
	FastNoiseSIMD* noise;
};
