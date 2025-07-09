#include "saving/worldsaver.h"
#include "GameState/PlayingGameState.h"
#include "world.h"
#include "saving/filewriter.h"
#include "saving/savedworldinfo.h"
#include <filesystem>

WorldSaver::WorldSaver(World* world, PlayingGameState* playingState)
{
	this->world = world;
	this->playingState = playingState;
}

void WorldSaver::saveWorld()
{
	savePath = "saves\\" + world->getWorldName();

	ensureDirectory();
	saveWorldInfoFile();
}

void WorldSaver::ensureDirectory()
{
	std::string path = std::string(RESOURCES_PATH + savePath);
	if (!std::filesystem::is_directory(path))
	{
		std::filesystem::create_directory(path);
	}
}

void WorldSaver::saveWorldInfoFile()
{
	glm::vec3 playerPos = playingState->getPlayer().getWorldPosition();

	SavedWorldInfo info = SavedWorldInfo{ world->getWorldSeed(), playerPos.x, playerPos.y, playerPos.z };

	FileWriter writer = FileWriter(savePath + "\\world.bin");
	writer.writeRaw(info);
}
