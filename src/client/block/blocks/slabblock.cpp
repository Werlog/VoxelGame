#include "block/blocks/slabblock.h"

SlabBlock::SlabBlock(BlockType blockType, const std::string& blockName, int textureId)
	: Block(blockType, blockName)
{
	bottomShape = std::make_shared<SlabBottomShape>(textureId, textureId, textureId, textureId, textureId, textureId);
}

SlabBlock::SlabBlock(BlockType blockType, const std::string& blockName, int sideTextureId, int topTextureId, int bottomTextureId)
	: Block(blockType, blockName)
{
	bottomShape = std::make_shared<SlabBottomShape>(sideTextureId, sideTextureId, sideTextureId, sideTextureId, topTextureId, bottomTextureId);
}

SlabBlock::SlabBlock(BlockType blockType, const std::string& blockName, int frontTextureId, int rightTextureId, int backTextureId, int leftTextureId, int topTextureId, int bottomTextureId)
	: Block(blockType, blockName)
{
	bottomShape = std::make_shared<SlabBottomShape>(frontTextureId, rightTextureId, backTextureId, leftTextureId, topTextureId, bottomTextureId);
}

void SlabBlock::onUpdate(glm::ivec3 position, BlockType block, World& world)
{

}

void SlabBlock::onRandomUpdate(glm::ivec3 position, BlockType block, World& world)
{

}

std::shared_ptr<BlockShape> SlabBlock::getBlockShape(BlockType block)
{
	return bottomShape;
}

AABB SlabBlock::getCollider(glm::ivec3 position, BlockType block)
{
	return AABB(position, glm::vec3(position) + glm::vec3(1.0f, 0.5f, 1.0f));
}

bool SlabBlock::isFullBlock() const
{
	return false;
}

bool SlabBlock::isTransparent() const
{
	return false;
}
