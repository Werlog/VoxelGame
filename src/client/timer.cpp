#include "timer.h"
#include <SDL2/SDL.h>

Timer::Timer()
{
	lastTicks = SDL_GetTicks64();
	deltaTime = 0.0f;
}

void Timer::tick()
{
	deltaTime = (SDL_GetTicks64() - lastTicks) / 1000.0;

	lastTicks = SDL_GetTicks64();
}