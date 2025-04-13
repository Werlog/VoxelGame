#include "world.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <iostream>
#include "profiling/codetimer.h"
#include <unordered_set>

World::World(const Shader& terrainShader)
{
	shaderModelLoc = glGetUniformLocation(terrainShader.getProgramHandle(), "model");

	noise = FastNoiseSIMD::NewFastNoiseSIMD(0); // TODO: Random seeds

	lastplayerCoord = ChunkCoord{ 0, 0, 0 };
}

void World::updateWorld(Player& player)
{
	ChunkCoord playerCoord = ChunkCoord::toChunkCoord(player.getPosition());

	if (playerCoord != lastplayerCoord)
	{
		updateLoadedChunks(playerCoord);
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

void World::updateLoadedChunks(ChunkCoord& newCoordinate)
{
	std::lock_guard lock(chunksMutex);

	std::queue<ChunkCoord> coordsToLoad;
	std::unordered_set<ChunkCoord> visited;

	coordsToLoad.push(newCoordinate);

	while (!coordsToLoad.empty())
	{
		ChunkCoord coord = coordsToLoad.front();
		coordsToLoad.pop();

		if (visited.find(coord) != visited.end())
			continue;

		if (loadedChunks.find(coord) == loadedChunks.end())
			loadChunk(coord);

		for (const auto& direction : chunkLoadDirections)
		{
			ChunkCoord neighbour = ChunkCoord{ coord.x + direction[0], coord.y + direction[1], coord.z + direction[2] };

			float xDiff = abs(newCoordinate.x - neighbour.x);
			float yDiff = abs(newCoordinate.y - neighbour.y);
			float zDiff = abs(newCoordinate.z - neighbour.z);

			float squareDist = xDiff * xDiff + zDiff * zDiff;

			if (squareDist <= RENDER_DISTANCE * RENDER_DISTANCE && yDiff <= RENDER_HEIGHT)
			{
				coordsToLoad.push(neighbour);
			}
		}

		visited.insert(coord);
	}

	for (auto it = loadedChunks.begin(); it != loadedChunks.end(); it++)
	{
		const ChunkCoord& coord = it->first;

		float xDiff = abs(newCoordinate.x - coord.x);
		float yDiff = abs(newCoordinate.y - coord.y);
		float zDiff = abs(newCoordinate.z - coord.z);

		float squareDist = xDiff * xDiff + zDiff * zDiff;
		if (squareDist > RENDER_DISTANCE * RENDER_DISTANCE || yDiff > RENDER_HEIGHT)
		{
			Chunk* chunk = it->second;
			delete chunk;
			it = loadedChunks.erase(it);
		}
	}

	lastplayerCoord = newCoordinate;
}

void World::loadChunk(ChunkCoord coordinate)
{
	if (loadedChunks.find(coordinate) != loadedChunks.end())
		return;

	Chunk* chunk = new Chunk(coordinate, this);
	chunk->generateChunk(noise);
	chunk->generateMesh(blockData);
	
	std::lock_guard lock(chunksMutex);

	loadedChunks.insert({ coordinate, chunk });

	for (const auto& direction : chunkLoadDirections)
	{
		ChunkCoord neighbour = ChunkCoord{ coordinate.x + direction[0], coordinate.y + direction[1], coordinate.z + direction[2] };

		Chunk* nChunk = getChunkByCoordinate(neighbour);
		if (nChunk != nullptr)
			nChunk->generateMesh(blockData);
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
