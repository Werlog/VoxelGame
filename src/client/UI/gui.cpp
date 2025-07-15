#include "UI/gui.h"

GUI::GUI(Game* game)
{
	this->game = game;
	this->shouldClose = false;
	this->nextGUI = nullptr;
}

void GUI::update(InputHandler& inputHandler, float deltaTime)
{
	for (auto it = uiElements.begin(); it != uiElements.end(); it++)
	{
		std::shared_ptr<UIElement> element = *it;
		element->update(inputHandler, deltaTime);
	}
}

void GUI::render()
{
	for (auto it = uiElements.begin(); it != uiElements.end(); it++)
	{
		std::shared_ptr<UIElement> element = *it;
		element->render(&game->getUIRenderer());
	}
}

std::shared_ptr<GUI> GUI::getNextGUI() const
{
	return nextGUI;
}

bool GUI::shouldGUIClose() const
{
	return shouldClose;
}

void GUI::closeGUI(std::shared_ptr<GUI> nextGUI)
{
	shouldClose = true;
	this->nextGUI = nextGUI;
}
