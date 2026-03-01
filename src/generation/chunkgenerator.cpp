#include "generation/chunkgenerator.h"
#include "world.h"
#include <random>
#include "generation/generationPhase/terrainShapePhase.h"
#include "generation/generationPhase/vegetationPhase.h"
#include "generation/generationPhase/featurePhase.h"

ChunkGenerator::ChunkGenerator(std::shared_ptr<Chunk> chunk, World& world, ChunkManager* chunkManager)
	: world(world)
{
	this->chunk = chunk;
	this->chunkManager = chunkManager;

	heightNoiseSet = nullptr;

	initPhases();
}

ChunkGenerator::~ChunkGenerator()
{
	if (heightNoiseSet != nullptr)
		FastNoiseSIMD::FreeNoiseSet(heightNoiseSet);
}

void ChunkGenerator::generate()
{
	createHeightNoiseSet();

	for (auto& phase : phases)
	{
		phase->generate();
	}
}

World& ChunkGenerator::getWorld()
{
	return world;
}

int ChunkGenerator::getHeightFromNoiseValue(float noiseValue) const
{
	return (int)floor(10.0f + noiseValue * 15.0f);
}

float ChunkGenerator::getNoiseAt(int blockX, int blockZ) const
{
	ChunkCoord coord = chunk->getCoord();

	int setX = blockX + CHUNK_SIZE_X;
	int setZ = blockZ + CHUNK_SIZE_Z;

	return heightNoiseSet[setZ + setX * CHUNK_SIZE_Z * 3];
}

int ChunkGenerator::getBlockHeightAt(int blockX, int blockZ) const
{
	return getHeightFromNoiseValue(getNoiseAt(blockX, blockZ));
}

void ChunkGenerator::initPhases()
{
	phases.emplace_back(std::make_unique<TerrainShapePhase>(*this, chunk));
	phases.emplace_back(std::make_unique<FeaturePhase>(*this, chunk));
	phases.emplace_back(std::make_unique<VegetationPhase>(*this, chunk));
}

void ChunkGenerator::createHeightNoiseSet()
{
	const ChunkCoord& coord = chunk->getCoord();
	heightNoiseSet = world.getNoise()->GetSimplexSet((coord.x - 1) * CHUNK_SIZE_X, 0, (coord.z - 1) * CHUNK_SIZE_Z, CHUNK_SIZE_X * 3, 1, CHUNK_SIZE_Z * 3);
}

unsigned int ChunkGenerator::getChunkSeed(const ChunkCoord& coord, unsigned int subsystemId)
{
	unsigned int seed = world.getNoise()->GetSeed();

	seed ^= coord.x * 73428767;
	seed ^= coord.y * 9122723;
	seed ^= coord.z * 1234567;

	seed ^= (seed << 13);
	seed ^= (seed >> 17);
	seed ^= (seed << 5);

	return seed ^ (subsystemId * 0x9e3779b9);
}
