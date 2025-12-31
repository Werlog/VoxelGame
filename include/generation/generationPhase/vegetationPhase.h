#pragma once

#include "generationPhase.h"

class VegetationPhase : public GenerationPhase
{
public:

	VegetationPhase(ChunkGenerator& chunkGenerator, std::shared_ptr<Chunk> chunk);

	void generate() override;
};
