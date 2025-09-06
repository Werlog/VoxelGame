#include "block/blocks/glasspaneblock.h"
#include "world.h"

GlassPaneBlock::GlassPaneBlock()
	: Block(BlockType::GLASS_PANE, "Glass Pane")
{
	this->forwardShape = std::make_shared<PaneForwardShape>(49);
	this->rightShape = std::make_shared<PaneRightShape>(49);
}

void GlassPaneBlock::onUpdate(glm::ivec3 position, BlockType block, World& world)
{
	// Check left/right
	if (world.getBlockAt(position.x + 1, position.y, position.z) != BlockType::AIR || world.getBlockAt(position.x - 1, position.y, position.z) != BlockType::AIR)
	{
		world.modifyBlockAt(position.x, position.y, position.z, Block::withState(blockType, BlockState::STATE_0), false, false);
	}
	else
	{
		world.modifyBlockAt(position.x, position.y, position.z, Block::withState(blockType, BlockState::STATE_1), false, false);
	}
}

void GlassPaneBlock::onRandomUpdate(glm::ivec3 position, BlockType block, World& world)
{

}

std::shared_ptr<BlockShape> GlassPaneBlock::getBlockShape(BlockType block)
{
	BlockState state = Block::getState(block);
	if (state == BlockState::STATE_0)
		return forwardShape;
	return rightShape;
}

AABB GlassPaneBlock::getCollider(glm::ivec3 position, BlockType block)
{
	BlockState state = Block::getState(block);

	glm::vec3 floatPos = glm::vec3(position);

	if (state == BlockState::STATE_0)
		return AABB(floatPos + glm::vec3(0.0f, 0.0f, 0.45f), floatPos + glm::vec3(1.0f, 1.0f, 0.55f));
	else
		return AABB(floatPos + glm::vec3(0.45f, 0.0f, 0.0f), floatPos + glm::vec3(0.55f, 1.0f, 1.00f));
}

int GlassPaneBlock::getBreakParticleTextureId() const
{
	return forwardShape->getFaces()[0].textureId;
}

bool GlassPaneBlock::isFullBlock() const
{
	return false;
}

bool GlassPaneBlock::isTransparent() const
{
	return true;
}
