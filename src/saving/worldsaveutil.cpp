#include "saving/worldsaveutil.h"
#include <filesystem>
#include "saving/filereader.h"
#include <sstream>

namespace saveutil
{
	namespace fs = std::filesystem;

	std::vector<std::string> getSavedWorlds()
	{
		std::vector<std::string> worlds = std::vector<std::string>();

		fs::path worldsPath = fs::path(RESOURCES_PATH "/saves");

		if (!fs::is_directory(worldsPath))
		{
			fs::create_directory(worldsPath);
			return worlds;
		}

		for (const fs::directory_entry& entry : fs::directory_iterator(worldsPath))
		{
			if (!fs::is_directory(entry))
				continue;

			worlds.push_back(entry.path().filename().string());
		}

		return worlds;
	}

	bool getWorldInfo(std::string worldName, SavedWorldInfo* worldInfo)
	{
		fs::path path = fs::path("saves/" + worldName + "/world.bin");

		if (!fs::exists(RESOURCES_PATH "/" + path.string()))
		{
			return false;
		}

		FileReader reader = FileReader(path.string());
		reader.readRaw(*worldInfo);
		return true;
	}

	std::string getRegionPath(WorldRegion region, const std::string& worldPath)
	{
		std::ostringstream oss;
		oss << RESOURCES_PATH << worldPath << "/regions/" << "r." << region.x << "." << region.y << "." << region.z << ".bin";

		return oss.str();
	}
}