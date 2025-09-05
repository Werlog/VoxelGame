#include "block/block.h"
#include "world.h"

Block::Block(BlockType blockType, const std::string& blockName)
{
	this->blockType = blockType;
	this->blockName = blockName;
}

BlockPlaceResult Block::placeBlock(std::unique_ptr<glm::vec3> lookingPos, World& world)
{
	glm::vec3 centerPos = glm::vec3(floor(lookingPos->x) + 0.5f, floor(lookingPos->y) + 0.5f, floor(lookingPos->z) + 0.5f);
	glm::vec3 direction = glm::normalize((*lookingPos) - centerPos);

	glm::vec3 compass[] =
	{
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
	};

	glm::vec3& highestDir = compass[0];
	float highestDot = -1.1f;
	for (const glm::vec3& dir : compass)
	{
		float dot = glm::dot(dir, direction);

		if (dot > highestDot)
		{
			highestDot = dot;
			highestDir = dir;
		}
	}

	int placeX = (int)floor(centerPos.x + highestDir.x);
	int placeY = (int)floor(centerPos.y + highestDir.y);
	int placeZ = (int)floor(centerPos.z + highestDir.z);

	if (world.getBlockAt(placeX, placeY, placeZ) == BlockType::AIR)
	{
		return BlockPlaceResult
		{
			true,
			blockType,
			glm::ivec3(placeX, placeY, placeZ),
		};
	}

	return BlockPlaceResult
	{
		false,
		BlockType::AIR,
		glm::ivec3(0, 0, 0),
	};
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

BlockType Block::withState(BlockType blockType, BlockState state)
{
	return (BlockType)(blockType | state << 6);
}

BlockState Block::getState(BlockType blockType)
{
	return (BlockState)(blockType >> 6);
}

bool Block::isSimilar(BlockType first, BlockType second)
{
	return getRaw(first) == getRaw(second);
}
