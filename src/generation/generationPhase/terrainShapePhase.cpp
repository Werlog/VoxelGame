#include "generation/generationPhase/terrainShapePhase.h"
#include "generation/chunkgenerator.h"

TerrainShapePhase::TerrainShapePhase(ChunkGenerator& chunkGenerator, std::shared_ptr<Chunk> chunk)
	: GenerationPhase(chunkGenerator, chunk)
{

}

void TerrainShapePhase::generate()
{
	const ChunkCoord& coord = chunk->getCoord();

	float* heightNoiseSet = chunkGenerator.getHeightNoiseSet();
	for (int x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (int y = 0; y < CHUNK_SIZE_Y; y++)
		{
			for (int z = 0; z < CHUNK_SIZE_Z; z++)
			{
				int yPos = y + coord.y * CHUNK_SIZE_Y;
				int noiseIndex = z + x * CHUNK_SIZE_Z;

				int height = chunkGenerator.getHeightFromNoiseValue(heightNoiseSet[noiseIndex]);

				if (yPos == height)
				{
					chunk->setBlockAt(x, y, z, BlockType::GRASS);
				}
				else if (yPos > height - 3 && yPos < height)
				{
					chunk->setBlockAt(x, y, z, BlockType::DIRT);
				}
				else if (yPos <= height - 3)
				{
					chunk->setBlockAt(x, y, z, BlockType::STONE);
				}
			}
		}
	}
}