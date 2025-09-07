#pragma once

#include <vector>
#include <string>
#include "savedworldinfo.h"
#include "worldRegion.h"

namespace saveutil
{
	std::vector<std::string> getSavedWorlds();

	bool getWorldInfo(std::string worldName, SavedWorldInfo* worldInfo);

	std::string getRegionPath(WorldRegion region, const std::string& worldPath);
}
