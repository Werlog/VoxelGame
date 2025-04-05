#pragma once
#include "blockdata.h"

constexpr int CHUNK_SIZE_X = 32;
constexpr int CHUNK_SIZE_Y = 32;
constexpr int CHUNK_SIZE_Z = 32;

class Chunk
{
public:

	Chunk();

	void generateChunk();
	void generateMesh(BlockData& blockData);
	void createChunkMesh(std::vector<int>& faceData);
	void render();

	BlockType getBlockAt(int x, int y, int z);
private:
	BlockType blocks[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];
	int faceCount;

	unsigned int VAO;
	unsigned int SSBO;
};
