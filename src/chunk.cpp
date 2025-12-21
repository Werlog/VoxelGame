#include "chunk.h"

#include <vector>
#include <glad/glad.h>
#include "faceChecks.h"
#include "profiling/codetimer.h"
#include "world.h"

Chunk::Chunk(ChunkCoord coord, World* world)
{
	this->coord = coord;
	this->world = world;

	modified = false;

	VAO = 0;
	SSBO = 0;
	faceCount = 0;

	std::memset(blocks, BlockType::AIR, sizeof(blocks));
	std::memset(light, 0, sizeof(light));
	shouldUpdateMesh.store(false);
}

Chunk::~Chunk()
{
	unloadMesh();
}

void Chunk::updateLight(BlockData& blockData)
{
	std::memset(light, 0, sizeof(light));

	for (int x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (int z = 0; z < CHUNK_SIZE_Z; z++)
		{
			bool hitBlock = false;
			for (int y = CHUNK_SIZE_Y - 1; y >= 0; y--)
			{
				int index = x + y * CHUNK_SIZE_Y + z * CHUNK_SIZE_X * CHUNK_SIZE_Y;
				if (!hitBlock)
				{
					light[index] = 15;

					BlockType blockType = getBlockAt(x, y, z);

					const std::shared_ptr<Block>& block = blockData.getBlock(blockType);

					if (!block->isTransparent())
						hitBlock = true;
					continue;
				}
				light[index] = 12;
			}
		}
	}
}

void Chunk::generateMesh(BlockData& blockData)
{
	faceData.clear();

	faceData.reserve(CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);

	for (int x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (int y = 0; y < CHUNK_SIZE_Y; y++)
		{
			for (int z = 0; z < CHUNK_SIZE_Z; z++)
			{
				int index = x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y;
				BlockType current = blocks[index];

				if (current == BlockType::AIR) continue;

				const std::shared_ptr<Block>& block = blockData.getBlock(current);
				std::shared_ptr<BlockShape> shape = block->getBlockShape(current);
				const std::vector<BlockShapeFace>& faces = shape->getFaces();

				int shapeIndex = blockData.getShapeIndex(shape->getShapeType());

				for (int i = 0; i < faces.size(); i++)
				{
					const BlockShapeFace& face = faces[i];

					int checkX = face.faceDirection.x + x;
					int checkY = face.faceDirection.y + y;
					int checkZ = face.faceDirection.z + z;

					if (face.cullable)
					{
						BlockType neighbour = getBlockAt(checkX, checkY, checkZ);
						if (neighbour == current)
							continue;
						const std::shared_ptr<Block>& neighbourBlock = blockData.getBlock(neighbour);

						if (neighbourBlock->isFullBlock() && !neighbourBlock->isTransparent())
							continue;
					}

					int ao = 0;
					if (block->isFullBlock())
						ao = calculateAO(x, y, z, i, face.faceDirection);

					createFace(x, y, z, face.textureId, getLightLevelAt(checkX, checkY, checkZ), shapeIndex, i, ao);
				}
			}
		}
	}

	faceCount = faceData.size();
}

void Chunk::createChunkMesh()
{
	if (faceCount <= 0)
	{
		return;
	}

	if (VAO == 0)
		glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	if (SSBO == 0)
		glGenBuffers(1, &SSBO);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);

	glBufferData(GL_SHADER_STORAGE_BUFFER, faceData.size() * sizeof(ChunkFace), faceData.data(), GL_DYNAMIC_DRAW);

	faceData.clear();
}

void Chunk::render(BlockData& blockData) const
{
	glBindVertexArray(VAO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, blockData.getShapeSSBO());

	glDrawArrays(GL_TRIANGLES, 0, faceCount * 6);
}

inline BlockType Chunk::getBlockAt(int x, int y, int z)
{
	if (x > CHUNK_SIZE_X - 1 || x < 0 || y > CHUNK_SIZE_Y - 1 || y < 0 || z > CHUNK_SIZE_Z - 1 || z < 0)
		return world->getBlockAt(coord.x * CHUNK_SIZE_X + x, coord.y * CHUNK_SIZE_Y + y, coord.z * CHUNK_SIZE_Z + z);
	int index = x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y;
	return blocks[index];
}

void Chunk::setBlockAt(int x, int y, int z, BlockType newBlock, bool isModification)
{
	if (x > CHUNK_SIZE_X - 1 || x < 0 || y > CHUNK_SIZE_Y - 1 || y < 0 || z > CHUNK_SIZE_Z - 1 || z < 0)
		return;
	int index = x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y;
	blocks[index] = newBlock;
	if (isModification)
	{
		modified = true;
	}
}

inline unsigned char Chunk::getLightLevelAt(int x, int y, int z)
{
	if (x > CHUNK_SIZE_X - 1 || x < 0 || y > CHUNK_SIZE_Y - 1 || y < 0 || z > CHUNK_SIZE_Z - 1 || z < 0)
		return 15;
	int index = x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y;
	return light[index];
}

inline void Chunk::setLightLevelAt(int x, int y, int z, unsigned char lightLevel)
{
	if (x > CHUNK_SIZE_X - 1 || x < 0 || y > CHUNK_SIZE_Y - 1 || y < 0 || z > CHUNK_SIZE_Z - 1 || z < 0)
		return;
	int index = x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y;
	light[index] = lightLevel;
}

const BlockType* Chunk::getChunkData()
{
	return blocks;
}

const ChunkCoord& Chunk::getCoord()
{
	return coord;
}

bool Chunk::wasModified() const
{
	return modified;
}

bool Chunk::hasMesh() const
{
	return VAO != 0;
}

void Chunk::unloadMesh()
{
	if (VAO != 0)
		glDeleteVertexArrays(1, &VAO);
	if (SSBO != 0)
		glDeleteBuffers(1, &SSBO);

	VAO = 0;
	SSBO = 0;
}

void Chunk::createFace(int x, int y, int z, int textureId, int lightLevel, int shapeIndex, int faceIndex, int ao)
{
	faceData.push_back(ChunkFace{ (x | y << 6 | z << 12 | textureId << 18 | lightLevel << 26), (shapeIndex | faceIndex << 4 | ao << 8) });
}

int Chunk::calculateAO(int blockX, int blockY, int blockZ, int faceIndex, const glm::ivec3& normal)
{
	auto calculateVertexAO = [this](bool sideOne, bool sideTwo, bool corner) -> int {
		if (sideOne && sideTwo)
			return 2;
		return sideOne + sideTwo + corner;
	};

	int ao = 0;

	std::array<bool, 8> neighbouringBlocks = getAONeighbours(blockX, blockY, blockZ, faceIndex);

	ao |= calculateVertexAO(neighbouringBlocks[3], neighbouringBlocks[6], neighbouringBlocks[5]);
	ao |= calculateVertexAO(neighbouringBlocks[6], neighbouringBlocks[4], neighbouringBlocks[7]) << 2;
	ao |= calculateVertexAO(neighbouringBlocks[3], neighbouringBlocks[1], neighbouringBlocks[0]) << 4;
	ao |= calculateVertexAO(neighbouringBlocks[1], neighbouringBlocks[4], neighbouringBlocks[2]) << 6;

	return ao;
}

std::array<bool, 8> Chunk::getAONeighbours(int x, int y, int z, int faceIndex)
{
	std::array<bool, 8> aoNeighbours = std::array<bool, 8>();

	auto checkBlock = [this](int x, int y, int z) -> bool {
		BlockType type = getBlockAt(x, y, z);

		if (type == BlockType::AIR)
			return false;

		const std::shared_ptr<Block>& block = world->getBlockData().getBlock(type);

		return block->isFullBlock() && !block->isTransparent();
	};

	int count = 0;
	switch (faceIndex)
	{
	case 0:
		for (int i = 1; i >= -1; i--)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (i == 0 && j == 0) continue;
				aoNeighbours[count++] = checkBlock(x + j, y + i, z + 1);
			}
		}
		break;
	case 1:
		for (int i = 1; i >= -1; i--)
		{
			for (int j = 1; j >= -1; j--)
			{
				if (i == 0 && j == 0) continue;
				aoNeighbours[count++] = checkBlock(x + 1, y + i, z + j);
			}
		}
		break;
	case 2:
		for (int i = 1; i >= -1; i--)
		{
			for (int j = 1; j >= -1; j--)
			{
				if (i == 0 && j == 0) continue;
				aoNeighbours[count++] = checkBlock(x + j, y + i, z - 1);
			}
		}
		break;
	case 3:
		for (int i = 1; i >= -1; i--)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (i == 0 && j == 0) continue;
				aoNeighbours[count++] = checkBlock(x - 1, y + i, z + j);
			}
		}
		break;
	case 4:
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (i == 0 && j == 0) continue;
				aoNeighbours[count++] = checkBlock(x + j, y + 1, z + i);
			}
		}
		break;
	case 5:
		for (int i = 1; i >= -1; i--)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (i == 0 && j == 0) continue;
				aoNeighbours[count++] = checkBlock(x + j, y - 1, z + i);
			}
		}
		break;
	default:
		break;
	}

	return aoNeighbours;
}
