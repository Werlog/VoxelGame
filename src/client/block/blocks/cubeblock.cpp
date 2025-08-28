#include "block/blocks/cubeblock.h"

CubeBlock::CubeBlock(BlockType blockType, const std::string& blockName, int textureId, bool isTransparent)
	: Block(blockType, blockName)
{
	cubeShape = std::make_shared<CubeShape>(textureId, textureId, textureId, textureId, textureId, textureId);
	this->transparent = isTransparent;
}

CubeBlock::CubeBlock(BlockType blockType, const std::string& blockName, int sideTextureId, int topTextureId, int bottomTextureId, bool isTransparent)
	: Block(blockType, blockName)
{
	cubeShape = std::make_shared<CubeShape>(sideTextureId, sideTextureId, sideTextureId, sideTextureId, topTextureId, bottomTextureId);
	this->transparent = isTransparent;
}

CubeBlock::CubeBlock(BlockType blockType, const std::string& blockName, int frontTextureId, int rightTextureId, int backTextureId, int leftTextureId, int topTextureId, int bottomTextureId, bool isTransparent)
	: Block(blockType, blockName)
{
	cubeShape = std::make_shared<CubeShape>(frontTextureId, rightTextureId, backTextureId, leftTextureId, topTextureId, bottomTextureId);
	this->transparent = isTransparent;
}

void CubeBlock::onUpdate(glm::ivec3 position, BlockType block, World& world)
{

}

void CubeBlock::onRandomUpdate(glm::ivec3 position, BlockType block, World& world)
{

}

std::shared_ptr<BlockShape> CubeBlock::getBlockShape(BlockType block)
{
	return cubeShape;
}

AABB CubeBlock::getCollider(glm::ivec3 position, BlockType block)
{
	return AABB(position, glm::vec3(position) + glm::vec3(1.0f));
}

bool CubeBlock::isFullBlock() const
{
	return true;
}

bool CubeBlock::isTransparent() const
{
	return transparent;
}
