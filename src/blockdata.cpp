#include "blockdata.h"

BlockData::BlockData()
{
	init();
}

const BlockProperties& BlockData::getTextureData(BlockType blockType)
{
	auto it = data.find(blockType);
	if (it == data.end()) return data[BlockType::AIR];

	return it->second;
}

int BlockData::getTextureIdFromFaceIndex(const BlockProperties& blockData, int index)
{
	switch (index)
	{
	case 0:
		return blockData.frontFaceTexId;
	case 1:
		return blockData.rightFaceTexId;
	case 2:
		return blockData.backFaceTexId;
	case 3:
		return blockData.leftFaceTexId;
	case 4:
		return blockData.topFaceTexId;
	case 5:
		return blockData.bottomFaceTexId;
	default:
		return blockData.frontFaceTexId;
	}
}

void BlockData::init()
{
	data.insert({ BlockType::AIR, BlockProperties{"Air", 0, 0, 0, 0, 0, 0} });
	data.insert({ BlockType::STONE, BlockProperties{"Stone", 1, 1, 1, 1, 1, 1} });
	data.insert({ BlockType::DIRT, BlockProperties{"Dirt", 2, 2, 2, 2, 2, 2} });
	data.insert({ BlockType::GRASS, BlockProperties{"Grass", 3, 3, 3, 3, 0, 2} });
	data.insert({ BlockType::BEDROCK, BlockProperties{"Bedrock", 17, 17, 17, 17, 17, 17} });
}
