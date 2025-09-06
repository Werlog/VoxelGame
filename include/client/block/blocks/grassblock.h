#pragma once

#include "cubeblock.h"

class GrassBlock : public CubeBlock
{
public:
	GrassBlock();

	void onRandomUpdate(glm::ivec3 position, BlockType block, World& world) override;
};
