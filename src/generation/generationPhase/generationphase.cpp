#include "generation/generationPhase/generationPhase.h"
#include "generation/chunkgenerator.h"

GenerationPhase::GenerationPhase(ChunkGenerator& chunkGenerator)
	: chunkGenerator(chunkGenerator)
{
	this->chunk = nullptr;
}

void GenerationPhase::setChunk(std::shared_ptr<Chunk> chunk)
{
	this->chunk = chunk;
}
