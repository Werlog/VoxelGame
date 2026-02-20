#include "generation/feature/oakTreeFeature.h"
#include "chunk.h"

OakTreeFeature::OakTreeFeature(glm::vec3 origin, int height, bool isModification)
	: GenerationFeature(origin)
{
	this->height = height;
	this->isModification = isModification;

	this->boundingBox = AABB(origin - glm::vec3(3, 0, 3), origin + glm::vec3(3, height + 1, 3));
}

void OakTreeFeature::setOrigin(glm::vec3 origin)
{
	this->origin = origin;
}

void OakTreeFeature::generate(std::shared_ptr<Chunk> chunk) const
{
	int xPos = (int)floor(origin.x);
	int yPos = (int)floor(origin.y);
	int zPos = (int)floor(origin.z);

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
							continue;
						}

						chunk->setBlockAt(leafX, leafY, leafZ, BlockType::OAK_LEAVES, isModification);
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
						continue;
					}

					if (chunk->getBlockAt(leafX, leafY, leafZ) == BlockType::AIR)
						chunk->setBlockAt(leafX, leafY, leafZ, BlockType::OAK_LEAVES, isModification);
				}
			}
		}
		if (y != height)
		{
			if (y + yPos >= CHUNK_SIZE_Y || y + yPos < 0)
				continue;
			chunk->setBlockAt(xPos, y + yPos, zPos, BlockType::OAK_LOG, isModification);
		}
	}
}
