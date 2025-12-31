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

	float* heightNoiseSet = chunkGenerator.getHeightNoiseSet();

	unsigned int seed = chunkGenerator.getChunkSeed(coord, 2);
	std::mt19937 rng(seed);
	std::uniform_int_distribution<int> grassChanceDist(0, 100);

	float* grassNoiseSet = chunkGenerator.getWorld().getGrassNoise()->GetPerlinSet(coord.x * CHUNK_SIZE_X, coord.y * CHUNK_SIZE_Y, coord.z * CHUNK_SIZE_Z, CHUNK_SIZE_X, 1, CHUNK_SIZE_Z, 9.00f);
	for (size_t x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (size_t z = 0; z < CHUNK_SIZE_Z; z++)
		{
			float grassValue = grassNoiseSet[z + x * CHUNK_SIZE_Z];

			if (grassValue < 0.3f)
				continue;

			int random = grassChanceDist(rng);

			if (random < 13)
			{
				int height = chunkGenerator.getHeightFromNoiseValue(heightNoiseSet[z + x * CHUNK_SIZE_Z]);
				int yPos = height - coord.y * CHUNK_SIZE_Y + 1;

				if (yPos >= CHUNK_SIZE_Y || yPos < 0)
					continue;

				BlockType generated = BlockType::TALL_GRASS;
				if (random < 4)
				{
					generated = BlockType::RED_FLOWER;
				}
				if (random < 2)
				{
					generated = BlockType::YELLOW_FLOWER;
				}

				chunk->setBlockAt(x, yPos, z, generated);
			}
		}
	}

	FastNoiseSIMD::FreeNoiseSet(grassNoiseSet);
}
