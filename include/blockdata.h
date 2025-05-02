#pragma once

#include <unordered_map>
#include <string>

enum BlockType : unsigned char
{
	AIR,
	STONE,
	DIRT,
	GRASS,
	BEDROCK,
};

struct BlockProperties
{
	std::string blockName;
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

	const BlockProperties& getTextureData(BlockType blockType);
	
	int getTextureIdFromFaceIndex(const BlockProperties& blockData, int index);
private:
	std::unordered_map<BlockType, BlockProperties> data;

	void init();
};
