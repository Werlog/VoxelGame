#pragma once

#include "../block.h"
#include "block/blockshape/crossshape.h"

class CrossBlock : public Block
{
public:
	CrossBlock(BlockType blockType, const std::string& blockName, int textureId, bool needsGrass);

	void onUpdate(glm::ivec3 position, BlockType block, World& world) override;
	void onRandomUpdate(glm::ivec3 position, BlockType block, World& world) override;

	std::shared_ptr<BlockShape> getBlockShape(BlockType block) override;
	AABB getCollider(glm::ivec3 position, BlockType block) override;
	int getBreakParticleTextureId() const override;

	bool isFullBlock() const override;
	bool isTransparent() const override;

	virtual BlockPlaceResult placeBlock(std::unique_ptr<glm::vec3> lookingPos, World& world) override;
protected:
	bool needsGrass;
	std::shared_ptr<CrossShape> crossShape;
};
