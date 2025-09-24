#include "block/blocks/cactusblock.h"
#include "world.h"

CactusBlock::CactusBlock()
	: Block(BlockType::CACTUS, "Cactus")
{
	cactusShape = std::make_shared<CactusShape>(70, 69, 71);
}

void CactusBlock::onUpdate(glm::ivec3 position, BlockType block, World& world)
{
	if (!validate(position, world))
	{
		world.modifyBlockAt(position.x, position.y, position.z, BlockType::AIR, true);
	}
}

void CactusBlock::onRandomUpdate(glm::ivec3 position, BlockType block, World& world)
{
	BlockType above = world.getBlockAt(position.x, position.y + 1, position.z);
	if (above != BlockType::AIR)
		return;

	int stemLength = calculateStemLength(position, world);
	if (stemLength < 3)
	{
		world.modifyBlockAt(position.x, position.y + 1, position.z, BlockType::CACTUS);
	}
}

std::shared_ptr<BlockShape> CactusBlock::getBlockShape(BlockType block)
{
	return cactusShape;
}

AABB CactusBlock::getCollider(glm::ivec3 position, BlockType block)
{
	glm::vec3 pos = glm::vec3(position);

	return AABB(pos + glm::vec3(0.0625f, 0.0f, 0.0625f), pos + glm::vec3(0.9375f, 1.0f, 0.9375f));
}

int CactusBlock::getBreakParticleTextureId() const
{
	return cactusShape->getFaces()[0].textureId;
}

bool CactusBlock::isFullBlock() const
{
	return false;
}

bool CactusBlock::isTransparent() const
{
	return false;
}

bool CactusBlock::isSolid() const
{
	return true;
}

BlockPlaceResult CactusBlock::placeBlock(std::unique_ptr<glm::vec3> lookingPos, World& world)
{
	BlockPlaceResult result = Block::placeBlock(std::move(lookingPos), world);

	if (validate(result.placedPosition, world))
		return result;

	return BlockPlaceResult
	{
		false,
		BlockType::AIR,
		glm::ivec3(0, 0, 0),
	};
}

bool CactusBlock::validate(glm::ivec3 pos, World& world)
{
	BlockType below = world.getBlockAt(pos.x, pos.y - 1, pos.z);

	if (below != BlockType::SAND && below != BlockType::CACTUS)
	{
		return false;
	}

	for (int x = -1; x <= 1; x++)
	{
		for (int z = -1; z <= 1; z++)
		{
			if (x != 0 && z != 0 || x == 0 && z == 0) continue;

			BlockType neighbour = world.getBlockAt(pos.x + x, pos.y, pos.z + z);
			if (neighbour != BlockType::AIR)
				return false;
		}
	}

	return true;
}

int CactusBlock::calculateStemLength(glm::ivec3 pos, World& world)
{
	int length = 0;

	do
	{
		pos.y -= 1;
		length++;
	} while (world.getBlockAt(pos.x, pos.y, pos.z) == BlockType::CACTUS);

	return length;
}
