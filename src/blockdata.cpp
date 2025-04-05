#include "blockdata.h"

BlockData::BlockData()
{
	init();
}

const TextureData& BlockData::getTextureData(BlockType blockType)
{
	auto it = textureData.find(blockType);
	if (it == textureData.end()) return textureData[BlockType::AIR];

	return it->second;
}

int BlockData::getTextureIdFromFaceIndex(const TextureData& blockData, int index)
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
	textureData.insert({ BlockType::AIR, TextureData{0, 0, 0, 0, 0, 0} });
	textureData.insert({ BlockType::STONE, TextureData{1, 1, 1, 1, 1, 1} });
	textureData.insert({ BlockType::DIRT, TextureData{2, 2, 2, 2, 2, 2} });
	textureData.insert({ BlockType::GRASS, TextureData{3, 3, 3, 3, 0, 2} });
	textureData.insert({ BlockType::BEDROCK, TextureData{17, 17, 17, 17, 17, 17} });
}
