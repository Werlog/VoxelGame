#pragma once

#include "chunk.h"
#include <memory>
#include "chunkmanager.h"

class World;

class ChunkGenerator
{
public:

	ChunkGenerator(std::shared_ptr<Chunk> chunk, World* world, ChunkManager* chunkManager);

	~ChunkGenerator();

	void generate();
private:
	std::shared_ptr<Chunk> chunk;
	World* world;
	ChunkManager* chunkManager;
	float* heightNoiseSet;

	void generateTerrainShape();
	void generateTrees();

	inline float getHeightFromNoise(float noiseValue);

	void generateTree(int xPos, int yPos, int zPos, int height);
	unsigned int getChunkSeed(const ChunkCoord& coord, unsigned int subsystemId);
	void createBlockMod(int x, int y, int z, BlockType newBlock);
};
