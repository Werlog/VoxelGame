#include "blockdata.h"

BlockData::BlockData()
{
	init();
}

const BlockProperties& BlockData::getBlockProperties(BlockType blockType)
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

const std::unordered_map<BlockType, BlockProperties>& BlockData::getData()
{
	return data;
}

void BlockData::init()
{
	data.insert({ BlockType::AIR, BlockProperties{"Air", 0, 0, 0, 0, 0, 0} });
	data.insert({ BlockType::STONE, BlockProperties{"Stone", 1, 1, 1, 1, 1, 1} });
	data.insert({ BlockType::OAK_PLANKS, BlockProperties{"Oak Planks", 4, 4, 4, 4, 4, 4} });
	data.insert({ BlockType::COBBLESTONE, BlockProperties{"Cobblestone", 16, 16, 16, 16, 16, 16} });
	data.insert({ BlockType::DIRT, BlockProperties{"Dirt", 2, 2, 2, 2, 2, 2} });
	data.insert({ BlockType::GRASS, BlockProperties{"Grass", 3, 3, 3, 3, 0, 2} });
	data.insert({ BlockType::BEDROCK, BlockProperties{"Bedrock", 17, 17, 17, 17, 17, 17} });
}
