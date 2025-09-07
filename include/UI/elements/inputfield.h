#pragma once
#include <glm/glm.hpp>
#include "UI/elements/uielement.h"
#include "UI/uirenderer.h"
#include <functional>

class InputField : public UIElement
{
public:

	InputField(Game* game, glm::vec2 relPos, glm::vec2 offset, glm::vec2 size);

	void update(InputHandler& inputHandler, float deltaTime) override;
	void render(UIRenderer* uiRenderer) override;

	void setOnChangedCallback(const std::function<void(const std::string&)>& func);

	std::string getText();
private:
	std::string text;
	std::function<void(const std::string&)> changedCallback;

	bool isSelected;
	bool showCursor;

	float cursorTimer;

	Font& minecraftFont;

	Texture& inputFieldTexture;

	bool containsNonASCIIChars(char* str);
};
