#pragma once

#include <unordered_map>

enum BlockType : unsigned char
{
	AIR,
	STONE,
	DIRT,
	GRASS,
	BEDROCK,
};

struct TextureData
{
	unsigned char frontFaceTexId;
	unsigned char rightFaceTexId;
	unsigned char backFaceTexId;
	unsigned char leftFaceTexId;
	unsigned char topFaceTexId;
	unsigned char bottomFaceTexId;
};

class BlockData
{
public:

	BlockData();

	const TextureData& getTextureData(BlockType blockType);
	
	int getTextureIdFromFaceIndex(const TextureData& blockData, int index);
private:
	std::unordered_map<BlockType, TextureData> textureData;

	void init();
};
