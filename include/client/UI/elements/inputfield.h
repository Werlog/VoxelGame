#pragma once
#include <glm/glm.hpp>
#include "UI/elements/uielement.h"
#include "UI/uirenderer.h"

class InputField : public UIElement
{
public:

	InputField(Game* game, glm::vec2 relPos, glm::vec2 offset, glm::vec2 size);

	void update(InputHandler& inputHandler, float deltaTime) override;
	void render(UIRenderer* uiRenderer) override;

	std::string getText();
private:
	std::string text;

	bool isSelected;
	bool showCursor;

	float cursorTimer;

	Font& minecraftFont;

	Texture& inputFieldTexture;

	bool containsNonASCIIChars(char* str);
};
