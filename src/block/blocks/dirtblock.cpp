#include "block/blocks/dirtblock.h"
#include "world.h"

DirtBlock::DirtBlock()
	: CubeBlock(BlockType::DIRT, "Dirt", 2, false)
{

}

void DirtBlock::onRandomUpdate(glm::ivec3 position, BlockType block, World& world)
{
	if (world.getBlockAt(position.x, position.y + 1, position.z) != BlockType::AIR)
		return;

	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			for (int z = -1; z <= 1; z++)
			{
				glm::ivec3 neighbourPos = glm::ivec3(position.x + x, position.y + y, position.z + z);

				if (world.getBlockAt(neighbourPos.x, neighbourPos.y, neighbourPos.z) == BlockType::GRASS)
				{
					world.modifyBlockAt(position.x, position.y, position.z, BlockType::GRASS);
					return;
				}
			}
		}
	}
}
