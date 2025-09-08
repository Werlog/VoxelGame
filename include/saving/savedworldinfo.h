#pragma once

#include <glm/glm.hpp>
#include "../block/block.h"

#pragma pack(push, 1)
struct SavedWorldInfo
{
	int worldSeed;
	float playerX, playerY, playerZ;
	float playerYaw, playerPitch;
	BlockType playerHotbar[9];
};
#pragma pack(pop)