#pragma once

#include "game.h"

class BaseGameState
{
public:
	BaseGameState(Game* game);

	virtual void update(float deltaTime, InputHandler& inputHandler) = 0;

	virtual void render() = 0;

	virtual void onEnter() = 0;
	virtual void onExit() = 0;
protected:
	Game* game;
};
