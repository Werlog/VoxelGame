#include "block/blocks/slabblock.h"

SlabBlock::SlabBlock(BlockType blockType, const std::string& blockName, int textureId)
	: Block(blockType, blockName)
{
	bottomShape = std::make_shared<SlabBottomShape>(textureId, textureId, textureId, textureId, textureId, textureId);
	topShape = std::make_shared<SlabTopShape>(textureId, textureId, textureId, textureId, textureId, textureId);
}

SlabBlock::SlabBlock(BlockType blockType, const std::string& blockName, int sideTextureId, int topTextureId, int bottomTextureId)
	: Block(blockType, blockName)
{
	bottomShape = std::make_shared<SlabBottomShape>(sideTextureId, sideTextureId, sideTextureId, sideTextureId, topTextureId, bottomTextureId);
	topShape = std::make_shared<SlabTopShape>(sideTextureId, sideTextureId, sideTextureId, sideTextureId, topTextureId, bottomTextureId);
}

SlabBlock::SlabBlock(BlockType blockType, const std::string& blockName, int frontTextureId, int rightTextureId, int backTextureId, int leftTextureId, int topTextureId, int bottomTextureId)
	: Block(blockType, blockName)
{
	bottomShape = std::make_shared<SlabBottomShape>(frontTextureId, rightTextureId, backTextureId, leftTextureId, topTextureId, bottomTextureId);
	topShape = std::make_shared<SlabTopShape>(frontTextureId, rightTextureId, backTextureId, leftTextureId, topTextureId, bottomTextureId);
}

void SlabBlock::onUpdate(glm::ivec3 position, BlockType block, World& world)
{

}

void SlabBlock::onRandomUpdate(glm::ivec3 position, BlockType block, World& world)
{

}

std::shared_ptr<BlockShape> SlabBlock::getBlockShape(BlockType block)
{
	BlockState state = Block::getState(block);
	if (state == BlockState::STATE_0)
		return bottomShape;
	return topShape;
}

AABB SlabBlock::getCollider(glm::ivec3 position, BlockType block)
{
	BlockState state = Block::getState(block);

	if (state == BlockState::STATE_0)
		return AABB(position, glm::vec3(position) + glm::vec3(1.0f, 0.5f, 1.0f));
	return AABB(glm::vec3(position) + glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(position) + glm::vec3(1.0f, 1.0f, 1.0f));
}

int SlabBlock::getBreakParticleTextureId() const
{
	return bottomShape->getFaces()[0].textureId;
}

bool SlabBlock::isFullBlock() const
{
	return false;
}

bool SlabBlock::isTransparent() const
{
	return false;
}

BlockPlaceResult SlabBlock::placeBlock(std::unique_ptr<glm::vec3> lookingPos, World& world)
{
	glm::vec3 centerPos = glm::vec3(floor(lookingPos->x) + 0.5f, floor(lookingPos->y) + 0.5f, floor(lookingPos->z) + 0.5f);

	float diff = lookingPos->y - centerPos.y;

	bool switchState = false;
	if ((diff >= 0.0f || diff < -0.4f) && diff < 0.48f)
	{
		switchState = true;
	}

	BlockPlaceResult result = Block::placeBlock(std::move(lookingPos), world);

	if (switchState)
	{
		result.placedBlock = Block::withState(blockType, BlockState::STATE_1);
	}

	return result;
}
