#pragma once

#include "crossblock.h"

class OakSaplingBlock : public CrossBlock
{
public:
	OakSaplingBlock();

	void onRandomUpdate(glm::ivec3 position, BlockType block, World& world) override;
};
