#pragma once

#include "chunkcoord.h"
#include <glm/glm.hpp>

constexpr int WORLD_REGION_SIZE = 16;

struct WorldRegion
{
	int x, y, z;

	bool operator ==(WorldRegion other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}

	bool operator !=(WorldRegion other) const
	{
		return x != other.x || y != other.y || z != other.z;
	}

	glm::ivec3 getRelativeChunkPosition(ChunkCoord coord)
	{
		int relX = coord.x - x * WORLD_REGION_SIZE;
		int relY = coord.y - y * WORLD_REGION_SIZE;
		int relZ = coord.z - z * WORLD_REGION_SIZE;

		return glm::ivec3(relX, relY, relZ);
	}

	static WorldRegion toWorldRegion(ChunkCoord coord)
	{
		int x = (int)floor(coord.x / (double)WORLD_REGION_SIZE);
		int y = (int)floor(coord.y / (double)WORLD_REGION_SIZE);
		int z = (int)floor(coord.z / (double)WORLD_REGION_SIZE);

		return WorldRegion{ x, y, z };
	}
};
