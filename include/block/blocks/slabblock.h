#pragma once

#include "../block.h"
#include "../blockshape/slabbottomshape.h"
#include "../blockshape/slabtopshape.h"

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
	int getBreakParticleTextureId() const override;

	bool isFullBlock() const override;
	bool isTransparent() const override;
	bool isSolid() const override;

	virtual BlockPlaceResult placeBlock(std::unique_ptr<glm::vec3> lookingPos, World& world) override;
protected:
	std::shared_ptr<SlabBottomShape> bottomShape;
	std::shared_ptr<SlabTopShape> topShape;
};
