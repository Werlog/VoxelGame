#pragma once

#include "../block.h"
#include "../blockshape/cubeshape.h"

class CubeBlock : public Block
{
public:
	CubeBlock(BlockType blockType, const std::string& blockName, int textureId, bool isTransparent);
	CubeBlock(BlockType blockType, const std::string& blockName, int sideTextureId, int topTextureId, int bottomTextureId, bool isTransparent);
	CubeBlock(BlockType blockType, const std::string& blockName, int frontTextureId, int rightTextureId, int backTextureId, int leftTextureId, int topTextureId, int bottomTextureId, bool isTransparent);

	void onUpdate(glm::ivec3 position, BlockType block, World& world) override;
	void onRandomUpdate(glm::ivec3 position, BlockType block, World& world) override;

	std::shared_ptr<BlockShape> getBlockShape(BlockType block) override;
	AABB getCollider(glm::ivec3 position, BlockType block) override;
	int getBreakParticleTextureId() const override;

	bool isFullBlock() const override;
	bool isTransparent() const override;
protected:
	bool transparent;
	std::shared_ptr<CubeShape> cubeShape;
};
