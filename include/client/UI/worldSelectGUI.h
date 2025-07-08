#pragma once

#include "gui.h"
#include "GameState/MainMenuGameState.h"

class WorldSelectGUI : public GUI
{
public:

	WorldSelectGUI(Game* game, MainMenuGameState* mainMenu);

protected:
	MainMenuGameState* mainMenu;

	void constructGUI() override;
};
