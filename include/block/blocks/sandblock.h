#pragma once

#include "cubeblock.h"

class SandBlock : public CubeBlock
{
public:
	SandBlock();

	virtual void onUpdate(glm::ivec3 position, BlockType block, World& world) override;
};
