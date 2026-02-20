#include "generation/chunkgenerator.h"
#include "world.h"
#include <random>
#include "generation/generationPhase/terrainShapePhase.h"
#include "generation/generationPhase/vegetationPhase.h"
#include "generation/generationPhase/treePhase.h"

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

	chunk->setChunkStatus(ChunkStatus::GENERATED);
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
	phases.emplace_back(std::make_unique<TreePhase>(*this, chunk));
	phases.emplace_back(std::make_unique<VegetationPhase>(*this, chunk));
}

void ChunkGenerator::createHeightNoiseSet()
{
	const ChunkCoord& coord = chunk->getCoord();
	heightNoiseSet = world.getNoise()->GetSimplexSet((coord.x - 1) * CHUNK_SIZE_X, 0, (coord.z - 1) * CHUNK_SIZE_Z, CHUNK_SIZE_X * 3, 1, CHUNK_SIZE_Z * 3);
}

void ChunkGenerator::generateTrees()
{
	/*
	const ChunkCoord& coord = chunk->getCoord();
	unsigned int seed = getChunkSeed(coord, 1);
	std::mt19937 rng(seed);
	std::uniform_int_distribution<> distr(1, 1000);

	float* treeNoiseSet = world.getTreeNoise()->GetPerlinSet(coord.x * CHUNK_SIZE_X, 0, coord.z * CHUNK_SIZE_Z, CHUNK_SIZE_X, 1, CHUNK_SIZE_Z, 1.0f);
	for (size_t x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (size_t z = 0; z < CHUNK_SIZE_Z; z++)
		{
			float treeValue = treeNoiseSet[z + x * CHUNK_SIZE_Z];
			if (treeValue < 0.0f)
				continue;
			int randomNumber = distr(rng);
			if (randomNumber < 6)
			{
				int height = getHeightFromNoise(heightNoiseSet[z + x * CHUNK_SIZE_Z]);
				int yPos = height + coord.y * CHUNK_SIZE_Y;

				if (yPos == height && height + 1 < CHUNK_SIZE_Y)
					generateTree(x, yPos + 1, z, std::max(randomNumber + 2, 6));
			}
		}
	}

	FastNoiseSIMD::FreeNoiseSet(treeNoiseSet);
	*/
}

void ChunkGenerator::generateTree(int xPos, int yPos, int zPos, int height)
{
	/*
	for (int y = 0; y <= height; y++)
	{
		if (y >= height - 3)
		{
			int size = y == height - 1 ? 1 : 2;

			if (y == height)
			{
				for (int x = -1; x <= 1; x++)
				{
					for (int z = -1; z <= 1; z++)
					{
						if (x != 0 && z != 0)
							continue;

						int leafX = xPos + x;
						int leafY = yPos + y;
						int leafZ = zPos + z;

						if (leafX >= CHUNK_SIZE_X || leafX < 0
							|| leafY >= CHUNK_SIZE_Y || leafY < 0
							|| leafZ >= CHUNK_SIZE_Z || leafZ < 0)
						{
							createBlockMod(leafX, leafY, leafZ, BlockType::OAK_LEAVES);
							continue;
						}

						chunk->setBlockAt(leafX, leafY, leafZ, BlockType::OAK_LEAVES);
					}
				}
				continue;
			}

			for (int x = -size; x <= size; x++)
			{
				for (int z = -size; z <= size; z++)
				{
					int leafX = xPos + x;
					int leafY = yPos + y;
					int leafZ = zPos + z;

					
					if (leafX >= CHUNK_SIZE_X || leafX < 0
						|| leafY >= CHUNK_SIZE_Y || leafY < 0
						|| leafZ >= CHUNK_SIZE_Z || leafZ < 0)
					{
						createBlockMod(leafX, leafY, leafZ, BlockType::OAK_LEAVES);
						continue;
					}
					
					chunk->setBlockAt(leafX, leafY, leafZ, BlockType::OAK_LEAVES);
				}
			}
		}
		if (y != height)
		{
			if (y + yPos >= CHUNK_SIZE_Y || y + yPos < 0)
			{
				createBlockMod(xPos, y + yPos, zPos, BlockType::OAK_LOG);
			}
			chunk->setBlockAt(xPos, y + yPos, zPos, BlockType::OAK_LOG);
		}
	}
	*/
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
