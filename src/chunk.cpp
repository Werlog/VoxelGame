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
	shouldUpdateMesh.store(false);
}

Chunk::~Chunk()
{
	if (VAO != 0)
		glDeleteVertexArrays(1, &VAO);
	if (SSBO != 0)
		glDeleteBuffers(1, &SSBO);
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

				const BlockProperties& textureData = blockData.getBlockProperties(blocks[index]);

				for (int i = 0; i < 6; i++)
				{
					int checkIndex = i * 3;
					int checkX = faceChecks[checkIndex] + x;
					int checkY = faceChecks[checkIndex + 1] + y;
					int checkZ = faceChecks[checkIndex + 2] + z;


					if (getBlockAt(checkX, checkY, checkZ) != BlockType::AIR)
						continue;

					int textureId = blockData.getTextureIdFromFaceIndex(textureData, i);

					faceData.push_back(createFace(x, y, z, textureId, i));
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

	glBufferData(GL_SHADER_STORAGE_BUFFER, faceData.size() * sizeof(int), faceData.data(), GL_DYNAMIC_DRAW);

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

void Chunk::setBlockAt(int x, int y, int z, BlockType newBlock)
{
	if (x > CHUNK_SIZE_X - 1 || x < 0 || y > CHUNK_SIZE_Y - 1 || y < 0 || z > CHUNK_SIZE_Z - 1 || z < 0)
		return;
	int index = x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y;
	blocks[index] = newBlock;
}

const ChunkCoord& Chunk::getCoord()
{
	return coord;
}

bool Chunk::hasMesh() const
{
	return VAO != 0;
}

inline int Chunk::createFace(int x, int y, int z, int textureId, int faceDirection)
{
	return (x | y << 6 | z << 12 | textureId << 18 | faceDirection << 26);
}
