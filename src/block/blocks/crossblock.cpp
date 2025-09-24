#include "block/blocks/crossblock.h"
#include "world.h"

CrossBlock::CrossBlock(BlockType blockType, const std::string& blockName, int textureId, bool needsGrass)
	: Block(blockType, blockName)
{
	this->needsGrass = needsGrass;
	crossShape = std::make_shared<CrossShape>(textureId);
}

void CrossBlock::onUpdate(glm::ivec3 position, BlockType block, World& world)
{
	if (!needsGrass)
		return;

	BlockType below = world.getBlockAt(position.x, position.y - 1, position.z);
	if (below != BlockType::GRASS && below != BlockType::DIRT)
	{
		world.modifyBlockAt(position.x, position.y, position.z, BlockType::AIR, true);
	}
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

bool CrossBlock::isSolid() const
{
	return false;
}

BlockPlaceResult CrossBlock::placeBlock(std::unique_ptr<glm::vec3> lookingPos, World& world)
{
	BlockPlaceResult result = Block::placeBlock(std::move(lookingPos), world);

	if (!needsGrass)
		return result;

	BlockType belowBlock = world.getBlockAt(result.placedPosition.x, result.placedPosition.y - 1, result.placedPosition.z);

	if (belowBlock == BlockType::GRASS || belowBlock == BlockType::DIRT)
		return result;

	return BlockPlaceResult
	{
		false,
		BlockType::AIR,
		glm::ivec3(0, 0, 0),
	};
}
