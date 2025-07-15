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
	OAK_LOG,
	OAK_LEAVES,
	GLASS,
	BEDROCK,
	WOOL,
	BOOKSHELF,
	BRICKS,
	SMOOTH_STONE,
	SAND,
	GRAVEL,
	CLAY,
	MOSSY_COBBLESTONE,
	OBSIDIAN,
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
	int biomeMask;
	bool isTransparent;
};

constexpr int biomeNoFaceMask = 0x00;
constexpr int biomeFrontFaceMask = 0x01;
constexpr int biomeRightFaceMask = 0x02;
constexpr int biomeBackFaceMask = 0x04;
constexpr int biomeLeftFaceMask = 0x08;
constexpr int biomeTopFaceMask = 0x10;
constexpr int biomeBottomFaceMask = 0x20;
constexpr int biomeAllFaceMask = 0x3F;

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
