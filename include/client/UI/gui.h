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

	void update(InputHandler& inputHandler, float deltaTime);
	void render();

	std::shared_ptr<GUI> getNextGUI() const;
	bool shouldGUIClose() const;
protected:
	Game* game;
	std::vector<std::shared_ptr<UIElement>> uiElements;

	virtual void constructGUI() = 0;

	void closeGUI(std::shared_ptr<GUI> nextGUI = nullptr);
private:
	std::shared_ptr<GUI> nextGUI;
	bool shouldClose;
};
