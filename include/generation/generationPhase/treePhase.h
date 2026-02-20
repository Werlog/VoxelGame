#pragma once

#include "generationPhase.h"
#include "../feature/oakTreeFeature.h"
#include <vector>

class TreePhase : public GenerationPhase
{
public:

	TreePhase(ChunkGenerator& chunkGenerator, std::shared_ptr<Chunk> chunk);

	void generate() override;
private:

	std::vector<OakTreeFeature> findTreePositions();
};
