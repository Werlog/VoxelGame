#pragma once

#include "game.h"
#include "hotbar.h"

class HUD
{
public:
	
	HUD(Game* game, PlayingGameState* playingState, BlockIcons* blockIcons);

	void update(InputHandler& inputHandler, float deltaTime);
	void render(UIRenderer* uiRenderer);

	Hotbar& getHotbar();
private:
	Game* game;

	Hotbar hotbar;
};
