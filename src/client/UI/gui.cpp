#include "UI/gui.h"

GUI::GUI(Game* game)
{
	this->game = game;
}

void GUI::update(InputHandler& inputHandler)
{
	for (auto it = uiElements.begin(); it != uiElements.end(); it++)
	{
		std::shared_ptr<UIElement> element = *it;
		element->update(inputHandler);
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
