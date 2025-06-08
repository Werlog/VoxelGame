#pragma once

#include "gui.h"
#include "GameState/MainMenuGameState.h"

class SingleplayerGUI : public GUI
{
public:

	SingleplayerGUI(Game* game, MainMenuGameState* mainMenu);

protected:
	MainMenuGameState* mainMenu;

	void constructGUI() override;
};
