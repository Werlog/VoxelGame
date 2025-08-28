#pragma once

#include "../block.h"
#include "../blockshape/slabbottomshape.h"

class SlabBlock : public Block
{
public:
	SlabBlock(BlockType blockType, const std::string& blockName, int textureId);
	SlabBlock(BlockType blockType, const std::string& blockName, int sideTextureId, int topTextureId, int bottomTextureId);
	SlabBlock(BlockType blockType, const std::string& blockName, int frontTextureId, int rightTextureId, int backTextureId, int leftTextureId, int topTextureId, int bottomTextureId);

	void onUpdate(glm::ivec3 position, BlockType block, World& world) override;
	void onRandomUpdate(glm::ivec3 position, BlockType block, World& world) override;

	std::shared_ptr<BlockShape> getBlockShape(BlockType block) override;
	AABB getCollider(glm::ivec3 position, BlockType block) override;

	bool isFullBlock() const override;
	bool isTransparent() const override;
protected:
	std::shared_ptr<SlabBottomShape> bottomShape;
};
