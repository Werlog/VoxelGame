#pragma once

#include <vector>
#include <string>
#include "savedworldinfo.h"

namespace saveutil
{
	std::vector<std::string> getSavedWorlds();

	bool getWorldInfo(std::string worldName, SavedWorldInfo* worldInfo);
}
