#pragma once

#include <unordered_map>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

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
	void mouseUpdate(SDL_Event& event);
	void scrollUpdate(SDL_Event& event);
	void textInputUpdate(SDL_Event& event);

	bool getKey(int keycode) const;
	bool getKeyUp(int keycode) const;
	bool getKeyDown(int keycode) const;

	bool getMouseButton(unsigned char button) const;
	bool getMouseButtonDown(unsigned char button) const;
	bool getMouseButtonUp(unsigned char button) const;

	glm::vec2 getMousePosition();

	int getMouseScroll() const;

	char* getTextInput();
	void startTextInput();
	void endTextInput();
	bool isReceivingTextInput() const;
private:
	std::unordered_map<int, KeyState> keyStates;
	std::unordered_map<unsigned char, KeyState> mouseStates;
	bool receivingTextInput;

	char textInputBuffer[32];

	int scrollValue;
};
