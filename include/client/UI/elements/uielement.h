#pragma once

#include "UI/uirenderer.h"
#include "inputhandler.h"
#include <glm/glm.hpp>
#include "game.h"

class UIElement
{
public:
	UIElement(Game* game, glm::vec2 relPos, glm::vec2 offset, glm::vec2 size);

	virtual void update(InputHandler& inputHandler) = 0;
	virtual void render(UIRenderer* uiRenderer) = 0;

	glm::vec2 getPosition();
	bool checkHovered(InputHandler& inputHandler);
protected:
	Game* game;
	glm::vec2 positionOffset;
	glm::vec2 windowRelPos;
	glm::vec2 scale;
};
