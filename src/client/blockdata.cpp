#include "blockdata.h"
#include "block/blocks/cubeblock.h"
#include "block/blocks/slabblock.h"
#include "block/blocks/crossblock.h"
#include "block/blocks/grassblock.h"
#include <glad/glad.h>

BlockData::BlockData(Shader& chunkShader)
{
	shapeSSBO = 0;

	initShapes();
	init();
}

const std::shared_ptr<Block>& BlockData::getBlock(BlockType blockType)
{
	auto it = data.find(Block::getRaw(blockType));
	if (it == data.end()) return data[BlockType::AIR];

	return it->second;
}

const std::unordered_map<BlockType, std::shared_ptr<Block>>& BlockData::getData() const
{
	return data;
}

int BlockData::getShapeIndex(BlockShapeType shapeType) const
{
	auto it = shapeIndexMap.find(shapeType);
	if (it == shapeIndexMap.end()) return -1;

	return it->second;
}

unsigned int BlockData::getShapeSSBO() const
{
	return shapeSSBO;
}

void BlockData::initShapes()
{
	std::vector<BlockShapeSpecification> shapes = std::vector<BlockShapeSpecification>();

	CubeShape cube = CubeShape(0, 0, 0, 0, 0, 0);
	shapes.push_back(BlockShapeSpecification::convertBlockShape(cube));

	SlabBottomShape bottomSlab = SlabBottomShape(0, 0, 0, 0, 0, 0);
	shapes.push_back(BlockShapeSpecification::convertBlockShape(bottomSlab));

	SlabTopShape topSlab = SlabTopShape(0, 0, 0, 0, 0, 0);
	shapes.push_back(BlockShapeSpecification::convertBlockShape(topSlab));

	CrossShape cross = CrossShape(0);
	shapes.push_back(BlockShapeSpecification::convertBlockShape(cross));

	uploadShapes(shapes);

	for (size_t i = 0; i < shapes.size(); i++)
	{
		BlockShapeSpecification& spec = shapes[i];

		shapeIndexMap.insert({ spec.type, i });
	}
}

void BlockData::uploadShapes(const std::vector<BlockShapeSpecification>& shapes)
{
	glGenBuffers(1, &shapeSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, shapeSSBO);

	glBufferData(GL_SHADER_STORAGE_BUFFER, shapes.size() * sizeof(BlockShapeSpecification::BlockShapeData), nullptr, GL_STATIC_DRAW);

	for (size_t i = 0; i < shapes.size(); i++)
	{
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, i * sizeof(BlockShapeSpecification::BlockShapeData), sizeof(BlockShapeSpecification::BlockShapeData), &shapes[i].data);
	}
}

void BlockData::init()
{
	data.emplace(BlockType::AIR, std::make_shared<CubeBlock>(BlockType::AIR, "Air", 113, true));
	data.emplace(BlockType::STONE, std::make_shared<CubeBlock>(BlockType::STONE, "Stone", 1, false));
	data.emplace(BlockType::OAK_PLANKS, std::make_shared<CubeBlock>(BlockType::OAK_PLANKS, "Oak Planks", 4, false));
	data.emplace(BlockType::COBBLESTONE, std::make_shared<CubeBlock>(BlockType::COBBLESTONE, "Cobblestone", 16, false));
	data.emplace(BlockType::DIRT, std::make_shared<CubeBlock>(BlockType::DIRT, "Dirt", 2, false));
	data.emplace(BlockType::GRASS, std::make_shared<GrassBlock>());
	data.emplace(BlockType::OAK_LOG, std::make_shared<CubeBlock>(BlockType::OAK_LOG, "Oak Log", 20, 21, 21, false));
	data.emplace(BlockType::OAK_LEAVES, std::make_shared<CubeBlock>(BlockType::OAK_LEAVES, "Oak Leaves", 53, false));
	data.emplace(BlockType::COBBLESTONE_SLAB, std::make_shared<SlabBlock>(BlockType::COBBLESTONE_SLAB, "Cobblestone Slab", 16));
	data.emplace(BlockType::GLASS, std::make_shared<CubeBlock>(BlockType::GLASS, "Glass", 49, true));
	data.emplace(BlockType::BEDROCK, std::make_shared<CubeBlock>(BlockType::BEDROCK, "Bedrock", 17, true));
	data.emplace(BlockType::TALL_GRASS, std::make_shared<CrossBlock>(BlockType::TALL_GRASS, "Tall Grass", 106, true));
	data.emplace(BlockType::RED_FLOWER, std::make_shared<CrossBlock>(BlockType::RED_FLOWER, "Red Flower", 12, true));
	data.emplace(BlockType::YELLOW_FLOWER, std::make_shared<CrossBlock>(BlockType::YELLOW_FLOWER, "Yellow Flower", 13, true));
	data.emplace(BlockType::OAK_SAPLING, std::make_shared<CrossBlock>(BlockType::OAK_SAPLING, "Oak Sapling", 15, true));
	data.emplace(BlockType::SAND, std::make_shared<CubeBlock>(BlockType::SAND, "Sand", 18, false));
	data.emplace(BlockType::OAK_SLAB, std::make_shared<SlabBlock>(BlockType::OAK_SLAB, "Oak Slab", 4));
	data.emplace(BlockType::WOOL, std::make_shared<CubeBlock>(BlockType::WOOL, "Wool", 64, false));
	data.emplace(BlockType::BOOKSHELF, std::make_shared<CubeBlock>(BlockType::BOOKSHELF, "Bookshelf", 35, 4, 4, false));
	data.emplace(BlockType::MOSSY_COBBLESTONE, std::make_shared<CubeBlock>(BlockType::MOSSY_COBBLESTONE, "Mossy Cobblestone", 36, false));
	data.emplace(BlockType::OBSIDIAN, std::make_shared<CubeBlock>(BlockType::OBSIDIAN, "Obsidian", 37, false));
}
