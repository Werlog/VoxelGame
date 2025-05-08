#pragma once
#include <iostream>
#include <glm/glm.hpp>

constexpr int CHUNK_SIZE_X = 32;
constexpr int CHUNK_SIZE_Y = 32;
constexpr int CHUNK_SIZE_Z = 32;

struct ChunkCoord
{
	int x, y, z;

	bool operator ==(ChunkCoord other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}

	bool operator!=(ChunkCoord other) const
	{
		return x != other.x || y != other.y || z != other.z;
	}

	ChunkCoord operator+(const ChunkCoord& other) const
	{
		return ChunkCoord{ x + other.x, y + other.y, z + other.z };
	}

	ChunkCoord operator-(const ChunkCoord& other) const
	{
		return ChunkCoord{ x - other.x, y - other.y, z - other.z };
	}

	ChunkCoord& operator+=(const ChunkCoord& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}

	static ChunkCoord toChunkCoord(const glm::vec3& pos)
	{
		int x = (int)floor(pos.x / CHUNK_SIZE_X);
		int y = (int)floor(pos.y / CHUNK_SIZE_Y);
		int z = (int)floor(pos.z / CHUNK_SIZE_Z);

		return ChunkCoord{ x, y, z };
	}

	static ChunkCoord toChunkCoord(int xPos, int yPos, int zPos)
	{
		// Casting to float would break past 2^24
		int x = (int)floor(xPos / (double)CHUNK_SIZE_X);
		int y = (int)floor(yPos / (double)CHUNK_SIZE_Y);
		int z = (int)floor(zPos / (double)CHUNK_SIZE_Z);

		return ChunkCoord{ x, y, z };
	}
};

namespace std
{
	template<> struct hash<ChunkCoord> {
		size_t operator()(const ChunkCoord& coord) const noexcept
		{
			size_t h1 = std::hash<int>{}(coord.x);
			size_t h2 = std::hash<int>{}(coord.y);
			size_t h3 = std::hash<int>{}(coord.z);

			size_t seed = h1;
			seed ^= h2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= h3 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			return seed;
		}
	};
}
