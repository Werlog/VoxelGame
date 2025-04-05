#pragma once
#include "blockdata.h"
#include <glm/glm.hpp>

constexpr int CHUNK_SIZE_X = 32;
constexpr int CHUNK_SIZE_Y = 32;
constexpr int CHUNK_SIZE_Z = 32;

struct ChunkCoord
{
	int x, y, z;

	bool operator ==(ChunkCoord other)
	{
		return x == other.x && y == other.y && z == other.z;
	}

	bool operator!=(ChunkCoord other)
	{
		return x != other.x || y != other.y || z != other.z;
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
		int x = (int)floor(xPos / CHUNK_SIZE_X);
		int y = (int)floor(yPos / CHUNK_SIZE_Y);
		int z = (int)floor(zPos / CHUNK_SIZE_Z);

		return ChunkCoord{ x, y, z };
	}
};

class Chunk
{
public:

	Chunk(ChunkCoord coord);

	void generateChunk();
	void generateMesh(BlockData& blockData);
	void createChunkMesh(std::vector<int>& faceData);
	void render();

	inline BlockType getBlockAt(int x, int y, int z);
	const ChunkCoord& getCoord();
private:
	BlockType blocks[CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z];
	int faceCount;

	ChunkCoord coord;

	unsigned int VAO;
	unsigned int SSBO;
};
