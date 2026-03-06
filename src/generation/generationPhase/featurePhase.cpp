#include "generation/generationPhase/featurePhase.h"
#include "generation/chunkgenerator.h"
#include <random>

FeaturePhase::FeaturePhase(ChunkGenerator& chunkGenerator)
	: GenerationPhase(chunkGenerator)
{

}

void FeaturePhase::generate()
{
	std::vector<std::unique_ptr<GenerationFeature>> features = findFeaturePositions();

	AABB chunkAABB = AABB(glm::vec3(0), glm::vec3(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z));

	for (auto& feature : features)
	{
		if (!chunkAABB.isOverlapping(feature->getBoundingBox()))
			continue;

		feature->generate(chunk);
	}
}

std::vector<std::unique_ptr<GenerationFeature>> FeaturePhase::findFeaturePositions()
{
	auto features = std::vector<std::unique_ptr<GenerationFeature>>();

	ChunkCoord originCoord = chunk->getCoord();
	std::uniform_int_distribution<int> treeChanceDist(0, 500);
	std::mt19937 rng(0);

	for (int chunkX = -1; chunkX <= 1; chunkX++)
	{
		for (int chunkY = -1; chunkY <= 1; chunkY++)
		{
			for (int chunkZ = -1; chunkZ <= 1; chunkZ++)
			{
				ChunkCoord coord = originCoord + ChunkCoord{ chunkX, chunkY, chunkZ };

				int seed = chunkGenerator.getChunkSeed(coord, 1);
				rng.seed(seed);

				for (int x = 0; x < CHUNK_SIZE_X; x++)
				{
					for (int z = 0; z < CHUNK_SIZE_Z; z++)
					{
						int random = treeChanceDist(rng);

						if (random > 2)
							continue;

						int xPos = x + chunkX * CHUNK_SIZE_X;
						int zPos = z + chunkZ * CHUNK_SIZE_Z;

						int height = chunkGenerator.getBlockHeightAt(xPos, zPos);

						int ownerChunkY = height / CHUNK_SIZE_Y;
						if (ownerChunkY != coord.y)
							continue;

						features.push_back(std::make_unique<OakTreeFeature>(glm::vec3(xPos, height - originCoord.y * CHUNK_SIZE_Y + 1, zPos), 7 - random));
					}
				}
			}
		}
	}

	return features;
}
