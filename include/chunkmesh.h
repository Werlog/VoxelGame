#pragma once

#include <vector>

class Chunk;

typedef struct
{
	int data;
	int secondData;
} ChunkFace;

class ChunkMesh
{
public:
	std::vector<ChunkFace> faces;

	ChunkMesh() = default;
};
