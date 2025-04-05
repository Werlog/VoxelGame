#include "chunk.h"

#include <vector>
#include <glad/glad.h>
#include "faceChecks.h"

Chunk::Chunk()
{
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
				if (y == 30)
				{
					blocks[x][y][z] = BlockType::GRASS;
				}
				else if (y > 27 && y < 30)
				{
					blocks[x][y][z] = BlockType::DIRT;
				}
				else if (y <= 27)
				{
					blocks[x][y][z] = BlockType::STONE;
				}
			}
		}
	}
}

void Chunk::generateMesh(BlockData& blockData)
{
	std::vector<int> faceData;

	for (size_t x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (size_t y = 0; y < CHUNK_SIZE_Y; y++)
		{
			for (size_t z = 0; z < CHUNK_SIZE_Z; z++)
			{
				if (blocks[x][y][z] == BlockType::AIR) continue;

				const TextureData& textureData = blockData.getTextureData(blocks[x][y][z]);

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

BlockType Chunk::getBlockAt(int x, int y, int z)
{
	if (x > CHUNK_SIZE_X - 1 || x < 0 || y > CHUNK_SIZE_Y - 1 || y < 0 || z > CHUNK_SIZE_Z - 1 || z < 0)
		return BlockType::AIR;

	return blocks[x][y][z];
}
