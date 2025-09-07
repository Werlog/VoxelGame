#include "UI/elements/button.h"

Button::Button(Game* game, glm::vec2 relPos, glm::vec2 offset, glm::vec2 size, const std::string& buttonText, const std::function<void()>& pressedCallback)
	: UIElement(game, relPos, offset, size), idleTexture(game->getResourceManager().getTexture("textures\\UI\\button.png")),
		hoveredTexture(game->getResourceManager().getTexture("textures\\UI\\buttonHover.png")),
		disabledTexture(game->getResourceManager().getTexture("textures\\UI\\buttonDisabled.png")),
		minecraftFont(game->getResourceManager().getFont("fonts\\MinecraftRegular.otf"))
{
	this->text = buttonText;
	this->callback = pressedCallback;
	this->isHovered = false;
	this->disabled = false;
}

void Button::update(InputHandler& inputHandler, float deltaTime)
{
	if (disabled)
	{
		isHovered = false;
		return;
	}

	isHovered = checkHovered(inputHandler);

	if (isHovered && inputHandler.getMouseButtonUp(SDL_BUTTON_LEFT) && callback)
	{
		callback();
	}
}

void Button::render(UIRenderer* uiRenderer)
{
	glm::vec2 pos = getPosition();

	uiRenderer->renderText(minecraftFont, text, pos.x, pos.y - (scale.y / 3), 0.75f, disabled ? glm::vec3(0.85f) : glm::vec3(1.0f), TextAlignment::ALIGN_CENTER);
	uiRenderer->renderTexturedQuad(getButtonTexture(), pos, scale, glm::vec2(1.0f));
}

void Button::setCallback(const std::function<void()>& func)
{
	this->callback = func;
}

bool Button::isDisabled() const
{
	return disabled;
}

void Button::setDisabled(bool disabled)
{
	this->disabled = disabled;
}

Texture& Button::getButtonTexture() const
{
	if (disabled)
		return disabledTexture;
	
	return isHovered ? hoveredTexture : idleTexture;
}
