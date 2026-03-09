#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "texturesheet.h"
#include "inputhandler.h"
#include "camera.h"
#include "resourcemanager.h"
#include "UI/uirenderer.h"
#include "debugRenderer.h"
#include "fpscounter.h"

class BaseGameState;

class Game
{
public:
	static constexpr int windowWidth = 1280;
	static constexpr int windowHeight = 720;

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

	FpsCounter fpsCounter;
	
	Camera camera;

	bool running;

	void handleEvents();
};
