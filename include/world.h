#pragma once

#include "chunk.h"
#include <vector>
#include "shader.h"

constexpr int WORLD_SIZE_X = 10;
constexpr int WORLD_SIZE_Y = 2;
constexpr int WORLD_SIZE_Z = 10;

class World
{
public:

	World(const Shader& terrainShader);

	void generateWorld();
	void renderWorld();

private:
	BlockData blockData;
	std::vector<Chunk*> loadedChunks;
	unsigned int shaderModelLoc;
};
