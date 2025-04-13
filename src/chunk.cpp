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

	VAO = 0;
	SSBO = 0;
	faceCount = 0;

	std::memset(blocks, BlockType::AIR, sizeof(blocks));
}

Chunk::~Chunk()
{
	if (VAO != 0)
		glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &SSBO);
}

void Chunk::generateChunk(FastNoiseSIMD* noise)
{
	float* noiseSet = noise->GetSimplexFractalSet(coord.x * CHUNK_SIZE_X, coord.y * CHUNK_SIZE_Y, coord.z * CHUNK_SIZE_Z, CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z);
	for (int x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (int y = 0; y < CHUNK_SIZE_Y; y++)
		{
			for (int z = 0; z < CHUNK_SIZE_Z; z++)
			{
				int yPos = y + coord.y * CHUNK_SIZE_Y;
				int index = x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y;
				int noiseIndex = z + y * CHUNK_SIZE_Z + x * CHUNK_SIZE_Z * CHUNK_SIZE_Y;

				int density = (int)floor(10.0f + noiseSet[noiseIndex] * 10.0f);

				if (density > 9.0f)
					blocks[index] = BlockType::BEDROCK;
				else
					blocks[index] = BlockType::AIR;

				/*
				if (yPos == height)
				{
					blocks[index] = BlockType::GRASS;
				}
				else if (yPos > height - 3 && yPos < height)
				{
					blocks[index] = BlockType::DIRT;
				}
				else if (yPos <= height - 3)
				{
					blocks[index] = BlockType::STONE;
				}
				*/
			}
		}
	}

	FastNoiseSIMD::FreeNoiseSet(noiseSet);
}

void Chunk::generateMesh(BlockData& blockData)
{
	std::vector<int> faceData;

	faceData.reserve(CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);

	for (int x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (int y = 0; y < CHUNK_SIZE_Y; y++)
		{
			for (int z = 0; z < CHUNK_SIZE_Z; z++)
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
	if (VAO == 0)
		glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	if (SSBO == 0)
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
		return world->getBlockAt(coord.x * CHUNK_SIZE_X + x, coord.y * CHUNK_SIZE_Y + y, coord.z * CHUNK_SIZE_Z + z);
	int index = x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y;
	return blocks[index];
}

const ChunkCoord& Chunk::getCoord()
{
	return coord;
}
