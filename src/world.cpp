#include "world.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <iostream>

World::World(const Shader& terrainShader)
{
	shaderModelLoc = glGetUniformLocation(terrainShader.getProgramHandle(), "model");

	noise = FastNoiseSIMD::NewFastNoiseSIMD(0); // TODO: Random seeds
}

void World::generateWorld()
{
	for (int x = -WORLD_SIZE_X / 2; x < WORLD_SIZE_X / 2; x++)
	{
		for (int y = 0; y < WORLD_SIZE_Y; y++)
		{
			for (int z = -WORLD_SIZE_Z / 2; z < WORLD_SIZE_Z / 2; z++)
			{
				Chunk* chunk = new Chunk(ChunkCoord{x, y, z});
				chunk->generateChunk(noise);
				chunk->generateMesh(blockData);
				
				loadedChunks.push_back(chunk);
			}
		}
	}

	std::cout << "World Generated" << std::endl;
}

void World::renderWorld()
{
	for (auto it = loadedChunks.begin(); it != loadedChunks.end(); it++)
	{
		Chunk* chunk = *it;
		const ChunkCoord& coord = chunk->getCoord();
		glm::mat4 model = glm::mat4(1);

		model = glm::translate(model, glm::vec3(coord.x * CHUNK_SIZE_X, coord.y * CHUNK_SIZE_Y, coord.z * CHUNK_SIZE_Z));

		glUniformMatrix4fv(shaderModelLoc, 1, GL_FALSE, glm::value_ptr(model));

		chunk->render();
	}
}
