#include "block/blocks/oaksaplingblock.h"
#include "world.h"
#include "generation/feature/oakTreeFeature.h"
#include <random>

OakSaplingBlock::OakSaplingBlock()
	: CrossBlock(BlockType::OAK_SAPLING, "Oak Sapling", 15, true)
{

}

void OakSaplingBlock::onRandomUpdate(glm::ivec3 position, BlockType block, World& world)
{
	std::random_device rnd;
	std::mt19937 rng(rnd());
	std::uniform_int_distribution<int> heightDist(4, 7);
	int treeHeight = heightDist(rng);

	std::vector<ChunkCoord> affectedChunks = std::vector<ChunkCoord>();

	ChunkCoord origin = ChunkCoord::toChunkCoord(position);

	AABB checkAABB = AABB(glm::vec3(0), glm::vec3(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z));
	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			for (int z = -1; z <= 1; z++)
			{
				ChunkCoord coord = origin + ChunkCoord{ x, y, z };
				std::shared_ptr<Chunk> chunk = world.getChunkByCoordinate(coord);
				if (chunk == nullptr)
					continue;

				glm::vec3 relative = glm::vec3(position.x - coord.x * CHUNK_SIZE_X, position.y - coord.y * CHUNK_SIZE_Y, position.z - coord.z * CHUNK_SIZE_Z);

				OakTreeFeature tree = OakTreeFeature(relative, treeHeight, true);
				if (!checkAABB.isOverlapping(tree.getBoundingBox()))
					continue;

				tree.generate(chunk);
				affectedChunks.push_back(coord);
			}
		}
	}

	for (ChunkCoord& coord : affectedChunks)
	{
		world.getChunkManager().remeshChunk(coord);
	}
}
