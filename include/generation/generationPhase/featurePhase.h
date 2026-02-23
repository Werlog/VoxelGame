#pragma once

#include "generationPhase.h"
#include "../feature/oakTreeFeature.h"
#include <vector>

class FeaturePhase : public GenerationPhase
{
public:

	FeaturePhase(ChunkGenerator& chunkGenerator, std::shared_ptr<Chunk> chunk);

	void generate() override;
private:

	std::vector<std::unique_ptr<GenerationFeature>> findFeaturePositions();
};
