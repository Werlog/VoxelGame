#include "chunk.h"

#include <vector>
#include <glad/glad.h>
#include "faceChecks.h"
#include "profiling/codetimer.h"

Chunk::Chunk(ChunkCoord coord)
{
	this->coord = coord;
	VAO = 0;
	SSBO = 0;
	faceCount = 0;

	std::memset(blocks, BlockType::AIR, sizeof(blocks));
}

void Chunk::generateChunk()
{
	for (size_t x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (size_t y = 0; y < CHUNK_SIZE_Y; y++)
		{
			for (size_t z = 0; z < CHUNK_SIZE_Z; z++)
			{
				int yPos = y + coord.y * CHUNK_SIZE_Y;
				int index = x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y;

				if (yPos == 48)
				{
					blocks[index] = BlockType::GRASS;
				}
				else if (yPos > 44 && yPos < 48)
				{
					blocks[index] = BlockType::DIRT;
				}
				else if (yPos <= 44)
				{
					blocks[index] = BlockType::STONE;
				}
			}
		}
	}
}

void Chunk::generateMesh(BlockData& blockData)
{
	std::vector<int> faceData;

	faceData.reserve(CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);

	for (size_t x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (size_t y = 0; y < CHUNK_SIZE_Y; y++)
		{
			for (size_t z = 0; z < CHUNK_SIZE_Z; z++)
			{
				int index = x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y;

				if (blocks[index] == BlockType::AIR) continue;

				const TextureData& textureData = blockData.getTextureData(blocks[index]);

				for (size_t i = 0; i < 6; i++)
				{
					int checkIndex = i * 3;
					int checkX = faceChecks[checkIndex] + x;
					int checkY = faceChecks[checkIndex + 1] + y;
					int checkZ = faceChecks[checkIndex + 2] + z;

					if (getBlockAt(checkX, checkY, checkZ) != BlockType::AIR)
						continue;

					int textureId = blockData.getTextureIdFromFaceIndex(textureData, i);
					int face = (x | y << 6 | z << 12 | textureId << 18 | i << 26);
					faceData.push_back(face);
				}
			}
		}
	}

	faceCount = faceData.size();

	createChunkMesh(faceData);
}

void Chunk::createChunkMesh(std::vector<int>& faceData)
{
	if (VAO != 0) return;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &SSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);

	glBufferData(GL_SHADER_STORAGE_BUFFER, faceData.size() * sizeof(int), faceData.data(), GL_DYNAMIC_DRAW);
}

void Chunk::render()
{
	glBindVertexArray(VAO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);

	glDrawArrays(GL_TRIANGLES, 0, faceCount * 6);
}

inline BlockType Chunk::getBlockAt(int x, int y, int z)
{
	if (x > CHUNK_SIZE_X - 1 || x < 0 || y > CHUNK_SIZE_Y - 1 || y < 0 || z > CHUNK_SIZE_Z - 1 || z < 0)
		return BlockType::AIR;
	int index = x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y;
	return blocks[index];
}

const ChunkCoord& Chunk::getCoord()
{
	return coord;
}
