#pragma once

#include "../block.h"
#include "../blockshape/paneforwardshape.h"
#include "../blockshape/panerightshape.h"

class GlassPaneBlock : public Block
{
public:
	GlassPaneBlock();

	void onUpdate(glm::ivec3 position, BlockType block, World& world) override;
	void onRandomUpdate(glm::ivec3 position, BlockType block, World& world) override;

	std::shared_ptr<BlockShape> getBlockShape(BlockType block) override;
	AABB getCollider(glm::ivec3 position, BlockType block) override;
	int getBreakParticleTextureId() const override;

	bool isFullBlock() const override;
	bool isTransparent() const override;
	bool isSolid() const override;

	//virtual BlockPlaceResult placeBlock(std::unique_ptr<glm::vec3> lookingPos, World& world) override;
protected:
	std::shared_ptr<PaneForwardShape> forwardShape;
	std::shared_ptr<PaneRightShape> rightShape;
};
