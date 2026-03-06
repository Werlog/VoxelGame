#pragma once

#include "generationPhase.h"
#include "../feature/oakTreeFeature.h"
#include <vector>

class FeaturePhase : public GenerationPhase
{
public:

	FeaturePhase(ChunkGenerator& chunkGenerator);

	void generate() override;
private:

	std::vector<std::unique_ptr<GenerationFeature>> findFeaturePositions();
};
