#pragma once

#include <unordered_map>
#include <SDL2/SDL.h>

enum KeyState
{
	KEYDOWN,
	KEYUP,
	PRESSED,
	NOT_PRESSED,
};

class InputHandler 
{
public:

	InputHandler();

	void update();
	void keyUpdate(SDL_Event& event);

	bool getKey(int keycode) const;
	bool getKeyUp(int keycode) const;
	bool getKeyDown(int keycode) const;
private:
	std::unordered_map<int, KeyState> keyStates;
};
