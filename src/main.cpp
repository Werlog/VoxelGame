#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include "game.h"

int main()
{
	Game game = Game();
	if (!game.init())
	{
		std::cout << "Unable to start the game." << std::endl;
		return 1;
	}
	game.gameLoop();
	return 0;
}
