#include "world.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <iostream>
#include "profiling/codetimer.h"
#include <unordered_set>
#include <random>
#include <limits>

World::World(const Shader& terrainShader)
	: chunkManager(this)
{
	shaderModelLoc = glGetUniformLocation(terrainShader.getProgramHandle(), "model");

	setupWorldGen();

	lastplayerCoord = ChunkCoord{ 0, 0, 0 };
}

void World::updateWorld(Player& player)
{
	ChunkCoord playerCoord = ChunkCoord::toChunkCoord(player.getWorldPosition());

	if (playerCoord != lastplayerCoord)
	{
		updateLoadedChunks(playerCoord);
	}

	chunkManager.update();
}

void World::renderWorld(const ChunkCoord& playerCoord, const glm::vec3& cameraPos)
{
	const auto& loadedChunks = chunkManager.getLoadedChunks();
	std::lock_guard lock(chunkManager.getChunkMutex());
	for (auto it = loadedChunks.begin(); it != loadedChunks.end(); it++)
	{
		std::shared_ptr<Chunk> chunk = (*it).second;
		if (!chunk->hasMesh()) continue;

		const ChunkCoord& coord = (*it).first;
		glm::mat4 model = glm::mat4(1.0f);

		ChunkCoord relative = coord - playerCoord;

		model = glm::translate(model, glm::vec3(relative.x * CHUNK_SIZE_X, relative.y * CHUNK_SIZE_Y, relative.z * CHUNK_SIZE_Z + 1));

		glUniformMatrix4fv(shaderModelLoc, 1, GL_FALSE, glm::value_ptr(model));

		chunk->render();
	}
}

void World::updateLoadedChunks(ChunkCoord& newCoordinate)
{
	std::queue<ChunkCoord> coordsToLoad;
	std::unordered_set<ChunkCoord> visited;

	coordsToLoad.push(newCoordinate);
	const auto& loadedChunks = chunkManager.getLoadedChunks();

	while (!coordsToLoad.empty())
	{
		ChunkCoord coord = coordsToLoad.front();
		coordsToLoad.pop();

		if (visited.find(coord) != visited.end())
			continue;

		{
			std::lock_guard lock(chunkManager.getChunkMutex());
			if (loadedChunks.find(coord) == loadedChunks.end())
				loadChunk(coord);
		}
		
		for (const auto& direction : worldDirections)
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
			chunkManager.unloadChunk(coord);
		}
	}
	
	lastplayerCoord = newCoordinate;
}

void World::loadChunk(ChunkCoord coordinate)
{
	chunkManager.loadChunk(coordinate);
}

void World::remeshChunk(ChunkCoord coordinate)
{
	chunkManager.remeshChunk(coordinate);
}

BlockType World::getBlockAt(int x, int y, int z)
{
	ChunkCoord coord = ChunkCoord::toChunkCoord(x, y, z);
	std::shared_ptr<Chunk> chunk = getChunkByCoordinate(coord);
	if (chunk == nullptr)
		return BlockType::AIR;

	return chunk->getBlockAt(x - coord.x * CHUNK_SIZE_X, y - coord.y * CHUNK_SIZE_Y, z - coord.z * CHUNK_SIZE_Z);
}

void World::setBlockAt(int x, int y, int z, BlockType newBlock)
{
	ChunkCoord coord = ChunkCoord::toChunkCoord(x, y, z);
	std::shared_ptr<Chunk> chunk = getChunkByCoordinate(coord);
	if (chunk == nullptr)
		return;
	chunk->setBlockAt(x - coord.x * CHUNK_SIZE_X, y - coord.y * CHUNK_SIZE_Y, z - coord.z * CHUNK_SIZE_Z, newBlock);
}

void World::modifyBlockAt(int x, int y, int z, BlockType newBlock)
{
	setBlockAt(x, y, z, newBlock);

	ChunkCoord coord = ChunkCoord::toChunkCoord(x, y, z);

	chunkManager.remeshChunk(coord, true);

	for (const auto& direction : worldDirections)
	{
		int relX = x + direction[0];
		int relY = y + direction[1];
		int relZ = z + direction[2];

		ChunkCoord neighbourCoord = ChunkCoord::toChunkCoord(relX, relY, relZ);

		if (neighbourCoord != coord)
		{
			chunkManager.remeshChunk(neighbourCoord, true);
		}
	}
}

std::shared_ptr<Chunk> World::getChunkByCoordinate(ChunkCoord coord)
{
	return chunkManager.getLoadedChunk(coord);
}

FastNoiseSIMD* World::getNoise()
{
	return noise;
}

FastNoiseSIMD* World::getTreeNoise()
{
	return treeDensityNoise;
}

FastNoiseSIMD* World::getCaveNoise()
{
	return caveNoise;
}

int World::getWorldSeed()
{
	return seed;
}

BlockData& World::getBlockData()
{
	return blockData;
}

void World::setupWorldGen()
{
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<> distr(std::numeric_limits<int>::lowest(), std::numeric_limits<int>::max());

	seed = distr(rng);

	noise = FastNoiseSIMD::NewFastNoiseSIMD(seed);
	treeDensityNoise = FastNoiseSIMD::NewFastNoiseSIMD(seed + 1);
	caveNoise = FastNoiseSIMD::NewFastNoiseSIMD(seed - 1);

	std::cout << "World seed is: " << seed << std::endl;
}
