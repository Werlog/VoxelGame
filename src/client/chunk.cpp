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

void Chunk::updateLight()
{
	std::memset(light, 15, sizeof(light));

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

					if (getBlockAt(x, y, z) != BlockType::AIR)
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

				if (blocks[index] == BlockType::AIR) continue;

				const BlockProperties& blockProperties = blockData.getBlockProperties(blocks[index]);

				for (int i = 0; i < 6; i++)
				{
					int checkIndex = i * 3;
					int checkX = faceChecks[checkIndex] + x;
					int checkY = faceChecks[checkIndex + 1] + y;
					int checkZ = faceChecks[checkIndex + 2] + z;

					BlockType neighbour = getBlockAt(checkX, checkY, checkZ);
					const BlockProperties& neighbourProperties = blockData.getBlockProperties(neighbour);

					if (!neighbourProperties.isTransparent || blocks[index] == neighbour)
						continue;

					int textureId = blockData.getTextureIdFromFaceIndex(blockProperties, i);

					createFace(x, y, z, textureId, i, blockProperties.biomeMask, 0, getLightLevelAt(checkX, checkY, checkZ));
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

void Chunk::render() const
{
	glBindVertexArray(VAO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);

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

void Chunk::createFace(int x, int y, int z, int textureId, int faceDirection, int faceMask, int biomeColorIndex, int lightLevel)
{
	faceData.push_back(ChunkFace{ (x | y << 6 | z << 12 | textureId << 18 | faceDirection << 26), (biomeColorIndex | faceMask << 4 | lightLevel << 10)});
}
