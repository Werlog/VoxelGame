#pragma once

#include "gui.h"
#include "GameState/MainMenuGameState.h"

class CreateWorldGUI : public GUI
{
public:

	CreateWorldGUI(Game* game, MainMenuGameState* mainMenu);

protected:
	MainMenuGameState* mainMenu;

	void constructGUI() override;

	bool checkWorldName(const std::string& worldName);
};
