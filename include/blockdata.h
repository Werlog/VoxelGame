#pragma once

#include <unordered_map>
#include <string>

enum BlockType : unsigned char
{
	AIR,
	STONE,
	OAK_PLANKS,
	COBBLESTONE,
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

	const BlockProperties& getBlockProperties(BlockType blockType);
	
	int getTextureIdFromFaceIndex(const BlockProperties& blockData, int index);

	const std::unordered_map<BlockType, BlockProperties>& getData();

private:
	std::unordered_map<BlockType, BlockProperties> data;

	void init();
};
