#include "world.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <iostream>
#include "profiling/codetimer.h"

World::World(const Shader& terrainShader)
{
	shaderModelLoc = glGetUniformLocation(terrainShader.getProgramHandle(), "model");

	noise = FastNoiseSIMD::NewFastNoiseSIMD(0); // TODO: Random seeds
}

void World::generateWorld()
{
	CodeTimer codeTimer = CodeTimer("World Generation");
	for (int x = -WORLD_SIZE_X / 2; x < WORLD_SIZE_X / 2; x++)
	{
		for (int y = -WORLD_SIZE_Y / 2; y < WORLD_SIZE_Y / 2; y++)
		{
			for (int z = -WORLD_SIZE_Z / 2; z < WORLD_SIZE_Z / 2; z++)
			{
				ChunkCoord coord = ChunkCoord{ x, y, z };
				Chunk* chunk = new Chunk(coord, this);
				chunk->generateChunk(noise);
				chunk->generateMesh(blockData);
				
				loadedChunks.insert({coord, chunk});
			}
		}
	}
}

void World::renderWorld()
{
	for (auto it = loadedChunks.begin(); it != loadedChunks.end(); it++)
	{
		Chunk* chunk = (*it).second;
		const ChunkCoord& coord = (*it).first;
		glm::mat4 model = glm::mat4(1);

		model = glm::translate(model, glm::vec3(coord.x * CHUNK_SIZE_X, coord.y * CHUNK_SIZE_Y, coord.z * CHUNK_SIZE_Z));

		glUniformMatrix4fv(shaderModelLoc, 1, GL_FALSE, glm::value_ptr(model));

		chunk->render();
	}
}

BlockType World::getBlockAt(int x, int y, int z)
{
	ChunkCoord coord = ChunkCoord::toChunkCoord(x, y, z);
	Chunk* chunk = getChunkByCoordinate(coord);
	if (chunk == nullptr)
		return BlockType::AIR;

	return chunk->getBlockAt(x - coord.x * CHUNK_SIZE_X, y - coord.y * CHUNK_SIZE_Y, z - coord.z * CHUNK_SIZE_Z);
}

Chunk* World::getChunkByCoordinate(ChunkCoord coord)
{
	auto it = loadedChunks.find(coord);
	if (it != loadedChunks.end())
		return it->second;

	return nullptr;
}
