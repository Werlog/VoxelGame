#include "UI/elements/label.h"

Label::Label(Game* game, glm::vec2 relPos, glm::vec2 offset, glm::vec2 size, const std::string& labelText, glm::vec3 labelColor, TextAlignment alignment)
	: UIElement(game, relPos, offset, size), minecraftFont(game->getResourceManager().getFont("fonts\\MinecraftRegular.otf"))
{
	this->text = labelText;
	this->textColor = labelColor;
	this->alignment = alignment;
}

void Label::update(InputHandler& inputHandler, float deltaTime)
{

}

void Label::render(UIRenderer* uiRenderer)
{
	glm::vec2 position = getPosition();

	uiRenderer->renderText(minecraftFont, text, position.x, position.y, scale.x, textColor, alignment);
}
