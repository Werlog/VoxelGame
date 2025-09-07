#pragma once

#include "UI/elements/uielement.h"
#include "texture.h"

class Image : public UIElement
{
public:

	Image(Game* game, glm::vec2 relPos, glm::vec2 offset, glm::vec2 size, Texture* texture, glm::vec4 color = glm::vec4(0), bool fillScreen = false);

	void update(InputHandler& inputHandler, float deltaTime) override;
	void render(UIRenderer* uiRenderer) override;
private:
	Texture* texture;
	glm::vec4 color;
	bool fillScreen;
};
