#pragma once
#include "blockdata.h"
#include <FastNoiseSIMD.h>
#include "chunkcoord.h"
#include <atomic>
#include <mutex>

class World;

typedef struct
{
	int data;
	int biomeData;
} ChunkFace;

class Chunk
{
public:
	std::atomic<bool> shouldUpdateMesh;

	Chunk(ChunkCoord coord, World* world);
	~Chunk();

	void updateLight(BlockData& blockData);
	void generateMesh(BlockData& blockData);
	void createChunkMesh();
	void render(BlockData& blockData) const;

	inline BlockType getBlockAt(int x, int y, int z);
	void setBlockAt(int x, int y, int z, BlockType newBlock, bool isModification = false);

	inline unsigned char getLightLevelAt(int x, int y, int z);
	inline void setLightLevelAt(int x, int y, int z, unsigned char lightLevel);

	const BlockType* getChunkData();

	const ChunkCoord& getCoord();

	bool wasModified() const;

	bool hasMesh() const;
	void unloadMesh();
private:
	BlockType blocks[CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z];
	unsigned char light[CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z];
	int faceCount;
	std::vector<ChunkFace> faceData;

	bool modified;
	
	World* world;

	ChunkCoord coord;

	unsigned int VAO;
	unsigned int SSBO;

	void createFace(int x, int y, int z, int textureId, int lightLevel, int shapeIndex, int faceIndex);
};
