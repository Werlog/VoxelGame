#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "texturesheet.h"
#include "inputhandler.h"
#include "camera.h"
#include "resourcemanager.h"

constexpr int windowWidth = 1280;
constexpr int windowHeight = 720;

class BaseGameState;

class Game
{
public:

	Game();

	bool init();

	void gameLoop();

	Camera& getCamera();

	void switchToState(BaseGameState* newState);
private:
	SDL_Window* window;
	SDL_GLContext context;

	BaseGameState* currentState;

	InputHandler inputHandler;
	ResourceManager resourceManager;
	Camera camera;

	bool running;

	void handleEvents();
};
