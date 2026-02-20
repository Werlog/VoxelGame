#include "generation/generationPhase/vegetationPhase.h"
#include "generation/chunkgenerator.h"
#include "world.h"
#include <random>

VegetationPhase::VegetationPhase(ChunkGenerator& chunkGenerator, std::shared_ptr<Chunk> chunk)
	: GenerationPhase(chunkGenerator, chunk)
{

}

void VegetationPhase::generate()
{
	const ChunkCoord& coord = chunk->getCoord();

	unsigned int seed = chunkGenerator.getChunkSeed(coord, 2);
	std::mt19937 rng(seed);
	std::uniform_int_distribution<int> grassChanceDist(0, 100);

	float* grassNoiseSet = chunkGenerator.getWorld().getGrassNoise()->GetPerlinSet(coord.x * CHUNK_SIZE_X, coord.y * CHUNK_SIZE_Y, coord.z * CHUNK_SIZE_Z, CHUNK_SIZE_X, 1, CHUNK_SIZE_Z, 6.00f);
	for (size_t x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (size_t z = 0; z < CHUNK_SIZE_Z; z++)
		{
			float grassValue = grassNoiseSet[z + x * CHUNK_SIZE_Z];

			if (grassValue < 0.1f)
				continue;

			int random = grassChanceDist(rng);

			if (random < 16)
			{
				int height = chunkGenerator.getBlockHeightAt(x, z);
				int yPos = height - coord.y * CHUNK_SIZE_Y + 1;

				if (yPos >= CHUNK_SIZE_Y || yPos < 0)
					continue;

				BlockType generated = BlockType::TALL_GRASS;
				if (random < 2)
				{
					generated = BlockType::RED_FLOWER;
				}
				if (random < 1)
				{
					generated = BlockType::YELLOW_FLOWER;
				}

				if (chunk->getBlockAt(x, yPos, z) == BlockType::AIR)
					chunk->setBlockAt(x, yPos, z, generated);
			}
		}
	}

	FastNoiseSIMD::FreeNoiseSet(grassNoiseSet);
}
