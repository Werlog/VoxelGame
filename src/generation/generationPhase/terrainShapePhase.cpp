#include "generation/generationPhase/terrainShapePhase.h"
#include "generation/chunkgenerator.h"
#include "world.h"
#include <iostream>
#include "profiling/codetimer.h"

TerrainShapePhase::TerrainShapePhase(ChunkGenerator& chunkGenerator)
	: GenerationPhase(chunkGenerator)
{
	heightNoiseSet = nullptr;
	weirdnessNoiseSet = nullptr;
	densityNoiseSet = nullptr;
}

TerrainShapePhase::~TerrainShapePhase()
{
	if (heightNoiseSet != nullptr)
		FastNoiseSIMD::FreeNoiseSet(heightNoiseSet);
	if (weirdnessNoiseSet != nullptr)
		FastNoiseSIMD::FreeNoiseSet(weirdnessNoiseSet);
	if (densityNoiseSet != nullptr)
		FastNoiseSIMD::FreeNoiseSet(densityNoiseSet);
}

void TerrainShapePhase::generate()
{
	const ChunkCoord& coord = chunk->getCoord();

	heightNoiseSet = chunkGenerator.getWorld().getNoise()->GetPerlinSet((coord.x - 1) * CHUNK_SIZE_X, 0, (coord.z - 1) * CHUNK_SIZE_Z, CHUNK_SIZE_X * 3, 1, CHUNK_SIZE_Z * 3, 2.0f);
	weirdnessNoiseSet = chunkGenerator.getWorld().getWeirdnessNoise()->GetPerlinSet((coord.x - 1) * CHUNK_SIZE_X, 0, (coord.z - 1) * CHUNK_SIZE_Z, CHUNK_SIZE_X * 3, 1, CHUNK_SIZE_Z * 3, 0.25f);
	densityNoiseSet = chunkGenerator.getWorld().getNoise()->GetPerlinSet((coord.x - 1) * CHUNK_SIZE_X, coord.y * CHUNK_SIZE_Y, (coord.z - 1) * CHUNK_SIZE_Z, CHUNK_SIZE_X * 3, CHUNK_SIZE_Y + surfaceBlocks, CHUNK_SIZE_Z * 3, 3.0f);

	for (int x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (int z = 0; z < CHUNK_SIZE_Z; z++)
		{
			int depth = getTopSurfaceDepth(x, z);
			int height = getHeightAt(x, z);

			for (int y = CHUNK_SIZE_Y - 1; y >= 0; y--)
			{
				int yPos = y + coord.y * CHUNK_SIZE_Y;

				if (yPos <= height)
				{
					depth++;
					chunk->setBlockAt(x, y, z, getBlockTypeFromDepth(depth));
				}
				else if (getDensityAt(x, y, z) >= densityThreshold)
				{
					depth++;
					chunk->setBlockAt(x, y, z, getBlockTypeFromDepth(depth));
				}
				else
				{
					depth = 0;
				}
			}
		}
	}

	populateHeightMap();
}

void TerrainShapePhase::populateHeightMap()
{
	ChunkCoord coord = chunk->getCoord();

	for (int x = -CHUNK_SIZE_X; x < CHUNK_SIZE_X * 2; x++)
	{
		for (int z = -CHUNK_SIZE_Z; z < CHUNK_SIZE_Z * 2; z++)
		{
			int height = getHeightAt(x, z) - coord.y * CHUNK_SIZE_Y;
			char topBlockY = 0;

			for (int y = CHUNK_SIZE_Y - 1; y >= std::max(0, height); y--)
			{
				if (getDensityAt(x, y, z) >= densityThreshold)
				{
					topBlockY = y;
					break;
				}
			}
			if (topBlockY == 0)
				topBlockY = std::max(0, height);

			chunkGenerator.setBlockHeightAt(x, z, topBlockY);
		}
	}
}

int TerrainShapePhase::getTopSurfaceDepth(int x, int z)
{
	int solidChain = 0;

	int height = getHeightAt(x, z);
	if (height - chunk->getCoord().y * CHUNK_SIZE_Y >= CHUNK_SIZE_Y)
		return surfaceBlocks;

	for (int i = surfaceBlocks; i >= 0; i--)
	{
		if (getDensityAt(x, CHUNK_SIZE_Y + i, z) >= densityThreshold)
		{
			solidChain++;
		}
		else
		{
			solidChain = 0;
		}
	}

	return solidChain;
}

BlockType TerrainShapePhase::getBlockTypeFromDepth(int depth)
{
	if (depth == 1)
		return BlockType::GRASS;
	if (depth <= surfaceBlocks)
		return BlockType::DIRT;

	return BlockType::STONE;
}

float TerrainShapePhase::getDensityAt(int x, int y, int z)
{
	int yPos = y + chunk->getCoord().y * CHUNK_SIZE_Y;

	float weirdness = (getWeirdnessNoiseAt(x, z) + 1.0f) * 0.5f;
	weirdness -= (float)yPos / 60.0f;
	weirdness = std::max(0.0f, weirdness);

	if (weirdness < 0.1f) return -1.0f;

	return getDensityNoiseAt(x, y, z) - 0.8f + weirdness;
}

float TerrainShapePhase::getHeightNoiseAt(int x, int z)
{
	int setX = x + CHUNK_SIZE_X;
	int setZ = z + CHUNK_SIZE_Z;

	return heightNoiseSet[setZ + setX * CHUNK_SIZE_Z * 3];
}

float TerrainShapePhase::getDensityNoiseAt(int x, int y, int z)
{
	int setX = x + CHUNK_SIZE_X;
	int setZ = z + CHUNK_SIZE_Z;

	return densityNoiseSet[setZ + y * CHUNK_SIZE_Z * 3 + setX * CHUNK_SIZE_Z * 3 * (CHUNK_SIZE_Y + surfaceBlocks)];
}

float TerrainShapePhase::getWeirdnessNoiseAt(int x, int z)
{
	int setX = x + CHUNK_SIZE_X;
	int setZ = z + CHUNK_SIZE_Z;

	return weirdnessNoiseSet[setZ + setX * CHUNK_SIZE_Z * 3];
}

int TerrainShapePhase::getHeightAt(int x, int z)
{
	return (int)floor(10.0f + getHeightNoiseAt(x, z) * 13.0f);
}
