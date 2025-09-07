#include "block/blocks/sandblock.h"
#include "world.h"

SandBlock::SandBlock()
	: CubeBlock(BlockType::SAND, "Sand", 18, false)
{

}

void SandBlock::onUpdate(glm::ivec3 position, BlockType block, World& world)
{
	glm::ivec3 toPos = position;
	constexpr int maxFall = 25;
	int fallenDistance = 0;

	while (world.getBlockAt(toPos.x, toPos.y - 1, toPos.z) == BlockType::AIR && fallenDistance <= maxFall)
	{
		toPos.y -= 1;
		fallenDistance++;
	}

	if (toPos == position)
		return;

	world.modifyBlockAt(toPos.x, toPos.y, toPos.z, BlockType::SAND);
	world.modifyBlockAt(position.x, position.y, position.z, BlockType::AIR, true);
}
