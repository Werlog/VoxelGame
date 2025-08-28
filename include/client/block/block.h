#pragma once

#include <string>
#include "blockshape/blockshape.h"
#include "AABB.h"
#include <memory>

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
	COBBLESTONE_SLAB,
	GLASS,
	BEDROCK,

	BLOCK_TYPE_LIMIT = 64,
};

enum BlockState : unsigned char
{
	STATE_0 = 0,
	STATE_1 = 1,
	STATE_2 = 2,
	STATE_3 = 3,
};

class World;

class Block
{
public:

	Block(BlockType blockType, const std::string& blockName);

	virtual void onUpdate(glm::ivec3 position, BlockType block, World& world) = 0;
	virtual void onRandomUpdate(glm::ivec3 position, BlockType block, World& world) = 0;

	virtual std::shared_ptr<BlockShape> getBlockShape(BlockType block) = 0;
	virtual AABB getCollider(glm::ivec3 position, BlockType block) = 0;
	virtual int getBreakParticleTextureId() const = 0;

	virtual bool isFullBlock() const = 0;
	virtual bool isTransparent() const = 0;

	const std::string& getName() const;
	BlockType getBlockType() const;

	static BlockType getRaw(BlockType blockType);
	static BlockState getState(BlockType blockType);
	static bool isSimilar(BlockType first, BlockType second);
protected:
	std::string blockName;
	BlockType blockType;
};
