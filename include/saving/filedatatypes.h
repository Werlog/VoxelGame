#pragma once

#include <glm/glm.hpp>
#include "../block/block.h"
#include <stdint.h>

constexpr int currentRegionVersion = 1;

#pragma pack(push, 1)
struct SavedWorldInfo
{
	int32_t worldSeed;
	float playerX, playerY, playerZ;
	float playerYaw, playerPitch;
	BlockType playerHotbar[9];
};

struct RegionFileHeader
{
	char signature[13];
	uint32_t regionVersion;
};
#pragma pack(pop)