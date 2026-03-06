#pragma once

#include "chunk.h"

class ChunkGenerator;

class GenerationPhase
{
public:
	
	GenerationPhase(ChunkGenerator& chunkGenerator);

	virtual ~GenerationPhase() = default;

	virtual void generate() = 0;

	void setChunk(std::shared_ptr<Chunk> chunk);
protected:
	ChunkGenerator& chunkGenerator;
	std::shared_ptr<Chunk> chunk;
};
