#include "UI/elements/image.h"

Image::Image(Game* game, glm::vec2 relPos, glm::vec2 offset, glm::vec2 size, Texture* texture, glm::vec4 color, bool fillScreen)
	: UIElement(game, relPos, offset, size)
{
	this->color = color;
	this->texture = texture;
	this->fillScreen = fillScreen;
}

void Image::update(InputHandler& inputHandler)
{
	if (fillScreen)
	{
		int width = game->getUIRenderer().getWindowWidth();
		int height = game->getUIRenderer().getWindowHeight();

		scale = glm::vec2(width / 2, height / 2);
	}
}

void Image::render(UIRenderer* uiRenderer)
{
	glm::vec2 position = getPosition();

	if (texture != nullptr)
		uiRenderer->renderTexturedQuad(*texture, position, scale, glm::vec2(1.0f));
	else
		uiRenderer->renderColoredQuad(color, position, scale);
}
