#pragma once

#include "gui.h"

class MainMenuGameState;

class MainMenuGUI : public GUI
{
public:

	MainMenuGUI(Game* game, MainMenuGameState* mainMenu);

protected:
	MainMenuGameState* mainMenu;

	void constructGUI() override;
};
