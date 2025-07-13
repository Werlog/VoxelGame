#include "UI/elements/HUD/hotbar.h"

Hotbar::Hotbar(Game* game, glm::vec2 relPos, glm::vec2 offset, glm::vec2 size, BlockIcons* blockIcons)
	: UIElement(game, relPos, offset, size),
	hotbarTexture(game->getResourceManager().getTexture("textures\\UI\\HUD\\hotbar.png")),
	selectedTexture(game->getResourceManager().getTexture("textures\\UI\\HUD\\hotbarSelected.png"))

{
	this->blockIcons = blockIcons;
}

void Hotbar::update(InputHandler& inputHandler, float deltaTime)
{

}

void Hotbar::render(UIRenderer* uiRenderer)
{
	glm::vec2 position = getPosition();

	for (int i = 1; i < 10; i++)
	{
		uiRenderer->renderTexturedQuad(blockIcons->getTextureForBlock((BlockType)i), position + glm::vec2(-scale.x + 37.0f + (i - 1) * 66.0f, 0.0f), glm::vec2(30.0f), glm::vec2(1.0f));
	}

	uiRenderer->renderTexturedQuad(hotbarTexture, position, scale, glm::vec2(1.0f));
}
