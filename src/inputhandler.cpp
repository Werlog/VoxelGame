#include "inputhandler.h"

InputHandler::InputHandler()
{

}

void InputHandler::update()
{
	for (auto it = keyStates.begin(); it != keyStates.end(); it++)
	{
		KeyState& state = it->second;

		if (state == KeyState::KEYDOWN)
			state = KeyState::PRESSED;
		else if (state == KeyState::KEYUP)
			state = KeyState::NOT_PRESSED;
	}

	for (auto it = mouseStates.begin(); it != mouseStates.end(); it++)
	{
		KeyState& state = it->second;

		if (state == KeyState::KEYDOWN)
			state = KeyState::PRESSED;
		else if (state == KeyState::KEYUP)
			state = KeyState::NOT_PRESSED;
	}
}

void InputHandler::keyUpdate(SDL_Event& event)
{
	if (event.type != SDL_KEYUP && event.type != SDL_KEYDOWN) return;

	keyStates[(int)event.key.keysym.sym] = event.type == SDL_KEYDOWN ? KeyState::KEYDOWN : KeyState::KEYUP;
}

void InputHandler::mouseUpdate(SDL_Event& event)
{
	if (event.type != SDL_MOUSEBUTTONDOWN && event.type != SDL_MOUSEBUTTONUP) return;

	mouseStates[(unsigned char)event.button.button] = event.type == SDL_MOUSEBUTTONDOWN ? KeyState::KEYDOWN : KeyState::KEYUP;
}

bool InputHandler::getKey(int keycode) const
{
	auto it = keyStates.find(keycode);
	if (it == keyStates.end()) return false;

	return it->second == KeyState::KEYDOWN || it->second == KeyState::PRESSED;
}

bool InputHandler::getKeyUp(int keycode) const
{
	auto it = keyStates.find(keycode);
	if (it == keyStates.end()) return false;

	return it->second == KeyState::KEYUP;
}

bool InputHandler::getKeyDown(int keycode) const
{
	auto it = keyStates.find(keycode);
	if (it == keyStates.end()) return false;

	return it->second == KeyState::KEYDOWN;
}

bool InputHandler::getMouseButton(unsigned char button) const
{
	auto it = mouseStates.find(button);
	if (it == mouseStates.end()) return false;

	return it->second == KeyState::KEYDOWN || it->second == KeyState::PRESSED;
}

bool InputHandler::getMouseButtonDown(unsigned char button) const
{
	auto it = mouseStates.find(button);
	if (it == mouseStates.end()) return false;

	return it->second == KeyState::KEYDOWN;
}

bool InputHandler::getMouseButtonUp(unsigned char button) const
{
	auto it = mouseStates.find(button);
	if (it == mouseStates.end()) return false;

	return it->second == KeyState::KEYUP;
}
