#pragma once

#include "generationPhase.h"

class TerrainShapePhase : public GenerationPhase 
{
public:
	static constexpr float densityThreshold = 0.0f;
	static constexpr int surfaceBlocks = 3;

	TerrainShapePhase(ChunkGenerator& chunkGenerator);

	void generate() override;
private:
	float* heightNoiseSet;
	float* weirdnessNoiseSet;
	float* densityNoiseSet;

	void populateHeightMap();

	int getTopSurfaceDepth(int x, int z);
	BlockType getBlockTypeFromDepth(int depth);

	float getDensityAt(int x, int y, int z);

	float getHeightNoiseAt(int x, int z);
	float getDensityNoiseAt(int x, int y, int z);
	float getWeirdnessNoiseAt(int x, int z);
	int getHeightAt(int x, int z);
};
