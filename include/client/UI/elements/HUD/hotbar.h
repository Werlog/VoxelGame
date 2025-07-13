#pragma once

#include "../uielement.h"
#include "../../blockicons.h"

class Hotbar : public UIElement
{
public:
	Hotbar(Game* game, glm::vec2 relPos, glm::vec2 offset, glm::vec2 size, BlockIcons* blockIcons);

	void update(InputHandler& inputHandler, float deltaTime) override;
	void render(UIRenderer* uiRenderer) override;
private:
	Texture& hotbarTexture;
	Texture& selectedTexture;
	BlockIcons* blockIcons;
};
