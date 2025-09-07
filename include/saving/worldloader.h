#pragma once

#include <memory>
#include "chunk.h"

class World;

class WorldLoader
{
public:

	WorldLoader(World* world);

	bool loadChunk(const std::shared_ptr<Chunk>& chunk);
private:
	World* world;
};
