#pragma once
#include "blockdata.h"
#include <FastNoiseSIMD.h>
#include "chunkcoord.h"
#include <atomic>
#include "chunkmesh.h"

class World;

class Chunk
{
public:
	std::shared_ptr<ChunkMesh> pendingMesh;
	std::shared_ptr<ChunkMesh> gpuMesh;

	Chunk(ChunkCoord coord, World* world);
	~Chunk();

	void updateLight(BlockData& blockData);
	void generateMesh(BlockData& blockData, std::shared_ptr<ChunkMesh> outputMesh);
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

	bool modified;
	
	World* world;

	ChunkCoord coord;

	unsigned int VAO;
	unsigned int SSBO;

	int calculateAO(int x, int y, int z, int faceIndex, const glm::ivec3& normal);

	std::array<bool, 8> getAONeighbours(int x, int y, int z, int faceIndex);
};
