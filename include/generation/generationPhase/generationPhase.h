#pragma once

#include "chunk.h"

class ChunkGenerator;

class GenerationPhase
{
public:
	
	GenerationPhase(ChunkGenerator& chunkGenerator, std::shared_ptr<Chunk> chunk);

	virtual void generate() = 0;
protected:
	ChunkGenerator& chunkGenerator;
	std::shared_ptr<Chunk> chunk;
};
