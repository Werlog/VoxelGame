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
	data.insert({ BlockType::AIR, BlockProperties{"Air", 30, 30, 30, 30, 30, 30, biomeNoFaceMask, true} });
	data.insert({ BlockType::STONE, BlockProperties{"Stone", 1, 1, 1, 1, 1, 1, biomeNoFaceMask, false} });
	data.insert({ BlockType::OAK_PLANKS, BlockProperties{"Oak Planks", 4, 4, 4, 4, 4, 4, biomeNoFaceMask, false} });
	data.insert({ BlockType::COBBLESTONE, BlockProperties{"Cobblestone", 16, 16, 16, 16, 16, 16, biomeNoFaceMask, false} });
	data.insert({ BlockType::DIRT, BlockProperties{"Dirt", 2, 2, 2, 2, 2, 2, biomeNoFaceMask, false} });
	data.insert({ BlockType::GRASS, BlockProperties{"Grass", 3, 3, 3, 3, 0, 2, biomeTopFaceMask, false} });
	data.insert({ BlockType::OAK_LOG, BlockProperties{"Oak Log", 20, 20, 20, 20, 21, 21, biomeNoFaceMask, false} });
	data.insert({ BlockType::OAK_LEAVES, BlockProperties{"Oak Leaves", 53, 53, 53, 53, 53, 53, biomeAllFaceMask, false} });
	data.insert({ BlockType::BEDROCK, BlockProperties{"Bedrock", 17, 17, 17, 17, 17, 17, biomeNoFaceMask, false} });
	data.insert({ BlockType::GLASS, BlockProperties{"Glass", 49, 49, 49, 49, 49, 49, biomeNoFaceMask, true} });
	data.insert({ BlockType::WOOL, BlockProperties{"Wool", 64, 64, 64, 64, 64, 64, biomeNoFaceMask, false} });
	data.insert({ BlockType::BOOKSHELF, BlockProperties{"Bookshelf", 35, 35, 35, 35, 4, 4, biomeNoFaceMask, false} });
	data.insert({ BlockType::BRICKS, BlockProperties{"Bricks", 7, 7, 7, 7, 7, 7, biomeNoFaceMask, false} });
	data.insert({ BlockType::SMOOTH_STONE, BlockProperties{"Smooth Stone", 5, 5, 5, 5, 6, 6, biomeNoFaceMask, false} });
	data.insert({ BlockType::SAND, BlockProperties{"Sand", 18, 18, 18, 18, 18, 18, biomeNoFaceMask, false} });
	data.insert({ BlockType::GRAVEL, BlockProperties{"Gravel", 19, 19, 19, 19, 19, 19, biomeNoFaceMask, false} });
	data.insert({ BlockType::CLAY, BlockProperties{"Clay", 72, 72, 72, 72, 72, 72, biomeNoFaceMask, false} });
	data.insert({ BlockType::MOSSY_COBBLESTONE, BlockProperties{"Mossy Cobblestone", 36, 36, 36, 36, 36, 36, biomeNoFaceMask, false} });
	data.insert({ BlockType::OBSIDIAN, BlockProperties{"Obsidian", 37, 37, 37, 37, 37, 37, biomeNoFaceMask, false} });
}
