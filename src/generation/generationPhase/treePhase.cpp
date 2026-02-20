#include "generation/generationPhase/treePhase.h"
#include "generation/chunkgenerator.h"
#include <random>

TreePhase::TreePhase(ChunkGenerator& chunkGenerator, std::shared_ptr<Chunk> chunk)
	: GenerationPhase(chunkGenerator, chunk)
{

}

void TreePhase::generate()
{
	std::vector<OakTreeFeature> trees = findTreePositions();

	AABB chunkAABB = AABB(glm::vec3(0), glm::vec3(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z));

	for (auto it = trees.begin(); it != trees.end(); it++)
	{
		OakTreeFeature& tree = *it;

		if (!chunkAABB.isOverlapping(tree.getBoundingBox()))
			continue;

		tree.generate(chunk);
	}
}

std::vector<OakTreeFeature> TreePhase::findTreePositions()
{
	std::vector<OakTreeFeature> oakTrees = std::vector<OakTreeFeature>();

	ChunkCoord originCoord = chunk->getCoord();

	for (int chunkX = -1; chunkX <= 1; chunkX++)
	{
		for (int chunkY = -1; chunkY <= 1; chunkY++)
		{
			for (int chunkZ = -1; chunkZ <= 1; chunkZ++)
			{
				ChunkCoord coord = originCoord + ChunkCoord{ chunkX, chunkY, chunkZ };

				int seed = chunkGenerator.getChunkSeed(coord, 1);
				std::mt19937 rng(seed);
				std::uniform_int_distribution<int> treeChanceDist(0, 500);

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

						oakTrees.push_back(OakTreeFeature(glm::vec3(xPos, height - originCoord.y * CHUNK_SIZE_Y + 1, zPos), 6 - random));
					}
				}
			}
		}
	}

	return oakTrees;
}
