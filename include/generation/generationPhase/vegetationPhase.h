#pragma once

#include "generationPhase.h"

class VegetationPhase : public GenerationPhase
{
public:

	VegetationPhase(ChunkGenerator& chunkGenerator);

	void generate() override;
};
