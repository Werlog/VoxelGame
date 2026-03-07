#pragma once

#include <list>
#include <unordered_map>
#include "chunkcoord.h"
#include "noisetile.h"

class NoiseCache
{
public:

private:
	std::unordered_map<ChunkCoord, NoiseTile> cacheMap;
};
