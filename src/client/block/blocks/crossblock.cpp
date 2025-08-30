#include "block/blocks/crossblock.h"

CrossBlock::CrossBlock(BlockType blockType, const std::string& blockName, int textureId)
	: Block(blockType, blockName)
{
	crossShape = std::make_shared<CrossShape>(textureId);
}

void CrossBlock::onUpdate(glm::ivec3 position, BlockType block, World& world)
{

}

void CrossBlock::onRandomUpdate(glm::ivec3 position, BlockType block, World& world)
{

}

std::shared_ptr<BlockShape> CrossBlock::getBlockShape(BlockType block)
{
	return crossShape;
}

AABB CrossBlock::getCollider(glm::ivec3 position, BlockType block)
{
	return AABB(glm::vec3(0.0f), glm::vec3(0.0f));
}

int CrossBlock::getBreakParticleTextureId() const
{
	return crossShape->getFaces()[0].textureId;
}

bool CrossBlock::isFullBlock() const
{
	return false;
}

bool CrossBlock::isTransparent() const
{
	return true;
}
