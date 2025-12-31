#pragma once

#include "generationPhase.h"

class TerrainShapePhase : public GenerationPhase 
{
public:

	TerrainShapePhase(ChunkGenerator& chunkGenerator, std::shared_ptr<Chunk> chunk);

	void generate() override;
private:

};
