#pragma once

#include "game.h"
#include <vector>
#include "UI/elements/uielement.h"
#include "inputhandler.h"
#include <memory>

class GUI
{
public:

	GUI(Game* game);

	void update(InputHandler& inputHandler);
	void render();
protected:
	Game* game;
	std::vector<std::shared_ptr<UIElement>> uiElements;

	virtual void constructGUI() = 0;
};
