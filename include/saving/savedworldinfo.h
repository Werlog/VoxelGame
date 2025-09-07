#pragma once

#include <glm/glm.hpp>

#pragma pack(push, 1)
struct SavedWorldInfo
{
	int worldSeed;
	float playerX, playerY, playerZ;
};
#pragma pack(pop)