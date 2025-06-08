#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "texturesheet.h"
#include "inputhandler.h"
#include "camera.h"
#include "resourcemanager.h"
#include "UI/uirenderer.h"
#include "debugRenderer.h"

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
	UIRenderer& getUIRenderer();
	DebugRenderer& getDebugRenderer();
	ResourceManager& getResourceManager();

	void switchToState(BaseGameState* newState);

	void quitGame();
	void setMousePosition(int x, int y);
private:
	SDL_Window* window;
	SDL_GLContext context;

	BaseGameState* currentState;

	InputHandler inputHandler;
	ResourceManager resourceManager;
	UIRenderer uiRenderer;
	DebugRenderer debugRenderer;
	
	Camera camera;

	bool running;

	void handleEvents();
};
