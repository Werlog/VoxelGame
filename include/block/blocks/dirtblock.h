#pragma once

#include "cubeblock.h"

class DirtBlock : public CubeBlock
{
public:
	DirtBlock();

	void onRandomUpdate(glm::ivec3 position, BlockType block, World& world) override;
};
