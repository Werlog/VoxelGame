#pragma once
#include "blockdata.h"
#include <FastNoiseSIMD.h>
#include "chunkcoord.h"

class World;

class Chunk
{
public:

	Chunk(ChunkCoord coord, World* world);
	~Chunk();

	void generateChunk(FastNoiseSIMD* noise);
	void generateMesh(BlockData& blockData);
	void createChunkMesh(std::vector<int>& faceData);
	void render();

	inline BlockType getBlockAt(int x, int y, int z);
	const ChunkCoord& getCoord();
private:
	BlockType blocks[CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z];
	int faceCount;
	
	World* world;

	ChunkCoord coord;

	unsigned int VAO;
	unsigned int SSBO;
};
