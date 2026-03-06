#pragma once

#include "../chunk.h"
#include "generationPhase/generationPhase.h"
#include <memory>

class World;
class ChunkManager;

class ChunkGenerator
{
public:
	ChunkGenerator(World& world, ChunkManager* chunkManager);

	~ChunkGenerator();

	void generate();

	World& getWorld();

	int getBlockHeightAt(int blockX, int blockZ) const;
	void setBlockHeightAt(int blockX, int blockZ, short height);

	unsigned int getChunkSeed(const ChunkCoord& coord, unsigned int subsystemId);

	void setChunk(std::shared_ptr<Chunk> chunk);
private:
	std::vector<std::unique_ptr<GenerationPhase>> phases;

	std::shared_ptr<Chunk> chunk;
	World& world;
	ChunkManager* chunkManager;

	short* heightSet;

	void initPhases();
};
