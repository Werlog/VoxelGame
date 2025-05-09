#include "chunkgenerator.h"
#include "world.h"
#include <random>

ChunkGenerator::ChunkGenerator(std::shared_ptr<Chunk> chunk, World* world, ChunkManager* chunkManager)
{
	this->chunk = chunk;
	this->world = world;
	this->chunkManager = chunkManager;

	heightNoiseSet = nullptr;
}

ChunkGenerator::~ChunkGenerator()
{
	if (heightNoiseSet != nullptr)
		FastNoiseSIMD::FreeNoiseSet(heightNoiseSet);
}

void ChunkGenerator::generate()
{
	generateTerrainShape();
	generateTrees();
}

void ChunkGenerator::generateTerrainShape()
{
	const ChunkCoord& coord = chunk->getCoord();
	heightNoiseSet = world->getNoise()->GetSimplexSet(coord.x * CHUNK_SIZE_X, 0, coord.z * CHUNK_SIZE_Z, CHUNK_SIZE_X, 1, CHUNK_SIZE_Z);
	for (int x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (int y = 0; y < CHUNK_SIZE_Y; y++)
		{
			for (int z = 0; z < CHUNK_SIZE_Z; z++)
			{
				int yPos = y + coord.y * CHUNK_SIZE_Y;
				int noiseIndex = z + x * CHUNK_SIZE_Z;

				int height = getHeightFromNoise(heightNoiseSet[noiseIndex]);

				if (yPos == height)
				{
					chunk->setBlockAt(x, y, z, BlockType::GRASS);
				}
				else if (yPos > height - 3 && yPos < height)
				{
					chunk->setBlockAt(x, y, z, BlockType::DIRT);
				}
				else if (yPos <= height - 3)
				{
					chunk->setBlockAt(x, y, z, BlockType::STONE);
				}
			}
		}
	}
}

void ChunkGenerator::generateTrees()
{
	const ChunkCoord& coord = chunk->getCoord();
	unsigned int seed = getChunkSeed(coord, 1);
	std::mt19937 rng(seed);
	std::uniform_int_distribution<> distr(1, 1000);

	for (size_t x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (size_t z = 0; z < CHUNK_SIZE_Z; z++)
		{
			int randomNumber = distr(rng);
			if (randomNumber < 4)
			{
				int height = getHeightFromNoise(heightNoiseSet[z + x * CHUNK_SIZE_Z]);
				int yPos = height + coord.y * CHUNK_SIZE_Y;

				if (yPos == height && height + 1 < CHUNK_SIZE_Y)
					generateTree(x, yPos + 1, z, std::max(randomNumber + 3, 5));
			}
		}
	}
}

inline float ChunkGenerator::getHeightFromNoise(float noiseValue)
{
	return (int)floor(10.0f + noiseValue * 15.0f);
}

void ChunkGenerator::generateTree(int xPos, int yPos, int zPos, int height)
{
	for (int y = 0; y <= height; y++)
	{
		if (y >= height - 2)
		{
			int size = y == height ? 1 : 2;
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
}

unsigned int ChunkGenerator::getChunkSeed(const ChunkCoord& coord, unsigned int subsystemId)
{
	unsigned int seed = world->getNoise()->GetSeed();

	seed ^= coord.x * 73428767;
	seed ^= coord.y * 9122723;
	seed ^= coord.z * 1234567;

	seed ^= (seed << 13);
	seed ^= (seed >> 17);
	seed ^= (seed << 5);

	return seed ^ (subsystemId * 0x9e3779b9);
}

void ChunkGenerator::createBlockMod(int x, int y, int z, BlockType newBlock)
{
	ChunkCoord diff = ChunkCoord::toChunkCoord(x, y, z);
	ChunkCoord newCoord = chunk->getCoord() + diff;

	int modX = x - diff.x * CHUNK_SIZE_X;
	int modY = y - diff.y * CHUNK_SIZE_Y;
	int modZ = z - diff.z * CHUNK_SIZE_Z;

	chunkManager->addBlockMod(newCoord, BlockMod{ modX, modY, modZ, newBlock });
}
