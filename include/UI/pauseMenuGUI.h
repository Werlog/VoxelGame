#pragma once

#include "gui.h"

class PlayingGameState;

class PauseMenuGUI : public GUI
{
public:

	PauseMenuGUI(Game* game, PlayingGameState* playingState);

protected:
	PlayingGameState* playingState;

	void constructGUI() override;
};
