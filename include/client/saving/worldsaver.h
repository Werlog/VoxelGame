#pragma once

class World;
class PlayingGameState;

#include <string>
#include "saving/worldRegion.h"
#include "chunk.h"

class WorldSaver
{
public:

	WorldSaver(World* world, PlayingGameState* playingState);

	void saveWorld();
private:
	World* world;
	PlayingGameState* playingState;

	std::string savePath;

	void ensureDirectory();
	void saveWorldInfoFile();
	void ensureChunksDirectory();
	void saveChunks();

	void saveChunk(std::shared_ptr<Chunk> chunk);
	std::string getRegionPath(WorldRegion region);
	void createEmptyRegionFile(const std::string& filePath);
};
