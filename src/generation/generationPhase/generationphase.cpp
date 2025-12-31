#include "generation/generationPhase/generationPhase.h"
#include "generation/chunkgenerator.h"

GenerationPhase::GenerationPhase(ChunkGenerator& chunkGenerator, std::shared_ptr<Chunk> chunk)
	: chunkGenerator(chunkGenerator)
{
	this->chunk = chunk;
}
