#pragma once

#include <SDL2/SDL.h>
#include <iostream>

constexpr int windowWidth = 800;
constexpr int windowHeight = 600;

class Game
{
public:

	Game();

	bool init();

	void gameLoop();
private:
	SDL_Window* window;
	SDL_GLContext context;
};