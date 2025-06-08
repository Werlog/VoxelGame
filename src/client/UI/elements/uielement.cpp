#include "UI/elements/uielement.h"

UIElement::UIElement(Game* game, glm::vec2 relPos, glm::vec2 offset, glm::vec2 size)
{
	this->game = game;
	this->windowRelPos = relPos;
	this->positionOffset = offset;
	this->scale = size;
}

glm::vec2 UIElement::getPosition()
{
	int width = game->getUIRenderer().getWindowWidth();
	int height = game->getUIRenderer().getWindowHeight();

	return glm::vec2(width * windowRelPos.x + positionOffset.x, height * windowRelPos.y + positionOffset.y);
}

bool UIElement::checkHovered(InputHandler& inputHandler)
{
	glm::vec2 elementPos = getPosition();
	glm::vec2 mousePos = inputHandler.getMousePosition();
	mousePos.y = game->getUIRenderer().getWindowHeight() - mousePos.y; // Element pos origin is bottom left, mouse position origin is top left

	glm::vec2 min = glm::vec2(elementPos.x - scale.x, elementPos.y - scale.y);
	glm::vec2 max = glm::vec2(elementPos.x + scale.x, elementPos.y + scale.y);

	return mousePos.x > min.x && mousePos.x < max.x
		&& mousePos.y > min.y && mousePos.y < max.y;
}
