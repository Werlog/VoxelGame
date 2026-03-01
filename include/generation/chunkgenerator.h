#pragma once

#include "../chunk.h"
#include "../chunkmanager.h"
#include "generationPhase/generationPhase.h"
#include <memory>

class World;

class ChunkGenerator
{
public:
	ChunkGenerator(std::shared_ptr<Chunk> chunk, World& world, ChunkManager* chunkManager);

	~ChunkGenerator();

	void generate();

	World& getWorld();

	int getHeightFromNoiseValue(float noiseValue) const;

	float getNoiseAt(int blockX, int blockZ) const;
	int getBlockHeightAt(int blockX, int blockZ) const;

	unsigned int getChunkSeed(const ChunkCoord& coord, unsigned int subsystemId);
private:
	std::vector<std::unique_ptr<GenerationPhase>> phases;

	std::shared_ptr<Chunk> chunk;
	World& world;
	ChunkManager* chunkManager;

	float* heightNoiseSet;

	void initPhases();

	void createHeightNoiseSet();
};
