#pragma once
#include "blockdata.h"
#include <FastNoiseSIMD.h>
#include "chunkcoord.h"
#include <atomic>
#include <mutex>

class World;

class Chunk
{
public:
	std::atomic<bool> shouldUpdateMesh;
	std::atomic<bool> toBeRemeshed; // If the chunk is currently queued for remeshing

	Chunk(ChunkCoord coord, World* world);
	~Chunk();

	void generateChunk(FastNoiseSIMD* noise);
	void generateMesh(BlockData& blockData);
	void createChunkMesh();
	void render() const;

	inline BlockType getBlockAt(int x, int y, int z);
	const ChunkCoord& getCoord();
	bool hasMesh() const;
private:
	BlockType blocks[CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z];
	int faceCount;
	std::vector<int> faceData;
	
	World* world;

	ChunkCoord coord;

	unsigned int VAO;
	unsigned int SSBO;

	inline int createFace(int x, int y, int z, int textureId, int faceDirection);
};
