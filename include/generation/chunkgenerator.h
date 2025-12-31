#pragma once

#include "../chunk.h"
#include "../chunkmanager.h"
#include "generationPhase/generationPhase.h"
#include <memory>

class World;

class ChunkGenerator
{
public:
	static constexpr float caveSize = 3.0f;

	ChunkGenerator(std::shared_ptr<Chunk> chunk, World& world, ChunkManager* chunkManager);

	~ChunkGenerator();

	void generate();

	World& getWorld();
	float* getHeightNoiseSet() const;

	int getHeightFromNoiseValue(float noiseValue);

	unsigned int getChunkSeed(const ChunkCoord& coord, unsigned int subsystemId);
private:
	std::vector<std::unique_ptr<GenerationPhase>> phases;

	std::shared_ptr<Chunk> chunk;
	World& world;
	ChunkManager* chunkManager;

	float* heightNoiseSet;

	void initPhases();

	void createHeightNoiseSet();
	void generateTrees();

	void generateTree(int xPos, int yPos, int zPos, int height);
};
