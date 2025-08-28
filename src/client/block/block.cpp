#include "block/block.h"

Block::Block(BlockType blockType, const std::string& blockName)
{
	this->blockType = blockType;
	this->blockName = blockName;
}

const std::string& Block::getName() const
{
	return blockName;
}

BlockType Block::getBlockType() const
{
	return blockType;
}

// Returns the block ID without its state
BlockType Block::getRaw(BlockType blockType)
{
	return (BlockType)(blockType & 63);
}

BlockState Block::getState(BlockType blockType)
{
	return (BlockState)(blockType >> 6);
}

bool Block::isSimilar(BlockType first, BlockType second)
{
	return getRaw(first) == getRaw(second);
}
