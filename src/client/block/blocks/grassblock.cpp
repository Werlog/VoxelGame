#include "block/blocks/grassblock.h"
#include "world.h"

GrassBlock::GrassBlock()
	: CubeBlock(BlockType::GRASS, "Grass", 3, 0, 2, false)
{

}

void GrassBlock::onRandomUpdate(glm::ivec3 position, BlockType block, World& world)
{
	const std::shared_ptr<Block>& above = world.getBlockData().getBlock(world.getBlockAt(position.x, position.y + 1, position.z));

	if (above->getBlockType() != BlockType::AIR && above->isFullBlock())
		world.modifyBlockAt(position.x, position.y, position.z, BlockType::DIRT);
}
