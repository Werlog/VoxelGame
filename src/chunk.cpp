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
	std::memset(light, 0, sizeof(light));
	shouldUpdateMesh.store(false);
}

Chunk::~Chunk()
{
	if (VAO != 0)
		glDeleteVertexArrays(1, &VAO);
	if (SSBO != 0)
		glDeleteBuffers(1, &SSBO);
}

void Chunk::updateLight()
{
	std::memset(light, 0, sizeof(light));

	int topBlocks[CHUNK_SIZE_X * CHUNK_SIZE_Z]; // Y levels of sky exposed air blocks
	std::memset(topBlocks, 0, sizeof(topBlocks));

	for (int x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (int z = 0; z < CHUNK_SIZE_Z; z++)
		{
			for (int y = CHUNK_SIZE_Y - 1; y >= 0; y--)
			{
				if (getBlockAt(x, y, z) != BlockType::AIR)
				{
					topBlocks[x + z * CHUNK_SIZE_X] = y + 1;
					break;
				}
				light[x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y] = 15;
			}
		}
	}

	for (int x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (int y = 0; y < CHUNK_SIZE_Y; y++)
		{
			for (int z = 0; z < CHUNK_SIZE_Z; z++)
			{
				if (y >= topBlocks[x + z * CHUNK_SIZE_X])
					continue;

				int blockIndex = x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y;

				if (blocks[blockIndex] != BlockType::AIR)
					continue;

				std::queue<glm::ivec4> positions;
				std::unordered_set<int> visited;
				positions.push(glm::ivec4(x, y, z, 0));

				while (!positions.empty())
				{
					glm::ivec4 pos = positions.front();
					positions.pop();

					int index = pos.x + pos.y * CHUNK_SIZE_X + pos.z * CHUNK_SIZE_X * CHUNK_SIZE_Y;

					if (visited.find(index) != visited.end())
						continue;

					visited.insert(index);

					if (index >= CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z || index < 0)
						continue;

					if (getBlockAt(pos.x, pos.y, pos.z) != BlockType::AIR)
						continue;

					if (pos.w >= 16)
					{
						light[blockIndex] = 0;
						break;
					}

					if (topBlocks[pos.x + pos.z * CHUNK_SIZE_X] <= pos.y)
					{
						light[blockIndex] = 15 - pos.w;
						break;
					}

					for (const auto& dir : worldDirections)
					{
						glm::ivec4 position = glm::ivec4(pos.x + dir[0], pos.y + dir[1], pos.z + dir[2], pos.w + 1);
						positions.push(position);
					}
				}
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

					if (getBlockAt(checkX, checkY, checkZ) != BlockType::AIR)
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

void Chunk::setBlockAt(int x, int y, int z, BlockType newBlock)
{
	if (x > CHUNK_SIZE_X - 1 || x < 0 || y > CHUNK_SIZE_Y - 1 || y < 0 || z > CHUNK_SIZE_Z - 1 || z < 0)
		return;
	int index = x + y * CHUNK_SIZE_X + z * CHUNK_SIZE_X * CHUNK_SIZE_Y;
	blocks[index] = newBlock;
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

const ChunkCoord& Chunk::getCoord()
{
	return coord;
}

bool Chunk::hasMesh() const
{
	return VAO != 0;
}

void Chunk::createFace(int x, int y, int z, int textureId, int faceDirection, int faceMask, int biomeColorIndex, int lightLevel)
{
	faceData.push_back(ChunkFace{ (x | y << 6 | z << 12 | textureId << 18 | faceDirection << 26), (biomeColorIndex | faceMask << 4 | lightLevel << 10)});
}
