#pragma once

#include "../block.h"
#include "../blockshape/cubeshape.h"

class CubeBlock : public Block
{
public:
	CubeBlock(BlockType blockType, const std::string& blockName, int textureId);
	CubeBlock(BlockType blockType, const std::string& blockName, int sideTextureId, int topTextureId, int bottomTextureId);
	CubeBlock(BlockType blockType, const std::string& blockName, int frontTextureId, int rightTextureId, int backTextureId, int leftTextureId, int topTextureId, int bottomTextureId);


	void onUpdate(glm::ivec3 position, BlockType block, World& world) override;
	void onRandomUpdate(glm::ivec3 position, BlockType block, World& world) override;

	std::shared_ptr<BlockShape> getBlockShape(BlockType block) override;
	AABB getCollider(glm::ivec3 position, BlockType block) override;

	bool isFullBlock() const override;
protected:
	std::shared_ptr<CubeShape> cubeShape;
};
