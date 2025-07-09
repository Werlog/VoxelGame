#pragma once

class World;
class PlayingGameState;

#include <string>

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
};
