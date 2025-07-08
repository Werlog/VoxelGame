#include "UI/elements/button.h"

Button::Button(Game* game, glm::vec2 relPos, glm::vec2 offset, glm::vec2 size, const std::string& buttonText, std::function<void()> pressedCallback)
	: UIElement(game, relPos, offset, size), idleTexture(game->getResourceManager().getTexture("textures\\UI\\button.png")),
		hoveredTexture(game->getResourceManager().getTexture("textures\\UI\\buttonHover.png")),
		minecraftFont(game->getResourceManager().getFont("fonts\\MinecraftRegular.otf"))
{
	this->text = buttonText;
	this->callback = pressedCallback;
	this->isHovered = false;
}

void Button::update(InputHandler& inputHandler, float deltaTime)
{
	isHovered = checkHovered(inputHandler);

	if (isHovered && inputHandler.getMouseButtonUp(SDL_BUTTON_LEFT))
	{
		callback();
	}
}

void Button::render(UIRenderer* uiRenderer)
{
	glm::vec2 pos = getPosition();

	uiRenderer->renderText(minecraftFont, text, pos.x, pos.y - (scale.y / 3), 0.75f, glm::vec3(1.0f), TextAlignment::ALIGN_CENTER);
	uiRenderer->renderTexturedQuad(isHovered ? hoveredTexture : idleTexture, pos, scale, glm::vec2(1.0f));
}
