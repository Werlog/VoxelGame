#pragma once
#include <glm/glm.hpp>
#include "UI/elements/uielement.h"
#include "UI/uirenderer.h"

class Label : public UIElement
{
public:

	Label(Game* game, glm::vec2 relPos, glm::vec2 offset, glm::vec2 size, const std::string& labelText, glm::vec3 labelColor, TextAlignment alignment);

	void update(InputHandler& inputHandler, float deltaTime) override;
	void render(UIRenderer* uiRenderer) override;

	void setText(const std::string& newText);
	std::string getText() const;
private:
	std::string text;
	glm::vec3 textColor;
	TextAlignment alignment;

	Font& minecraftFont;
};
