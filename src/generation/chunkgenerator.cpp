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

	heightSet = new char[CHUNK_SIZE_X * 3 * CHUNK_SIZE_Z * 3];
	std::memset(heightSet, 0, CHUNK_SIZE_X * 3 * CHUNK_SIZE_Z * 3);

	initPhases();
}

ChunkGenerator::~ChunkGenerator()
{
	delete[] heightSet;
}

void ChunkGenerator::generate()
{
	for (auto& phase : phases)
	{
		phase->generate();
	}
}

World& ChunkGenerator::getWorld()
{
	return world;
}

int ChunkGenerator::getBlockHeightAt(int blockX, int blockZ) const
{
	int setX = blockX + CHUNK_SIZE_X;
	int setZ = blockZ + CHUNK_SIZE_Z;

	return heightSet[setX + setZ * CHUNK_SIZE_X * 3];
}

void ChunkGenerator::setBlockHeightAt(int blockX, int blockZ, char height)
{
	int setX = blockX + CHUNK_SIZE_X;
	int setZ = blockZ + CHUNK_SIZE_Z;

	heightSet[setX + setZ * CHUNK_SIZE_X * 3] = height;
}

void ChunkGenerator::initPhases()
{
	phases.emplace_back(std::make_unique<TerrainShapePhase>(*this, chunk));
	phases.emplace_back(std::make_unique<FeaturePhase>(*this, chunk));
	phases.emplace_back(std::make_unique<VegetationPhase>(*this, chunk));
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
