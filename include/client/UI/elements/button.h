#include "uielement.h"
#include "texture.h"
#include <string>
#include <functional>

class Button : public UIElement
{
public:

	Button(Game* game, glm::vec2 relPos, glm::vec2 offset, glm::vec2 size, const std::string& buttonText, const std::function<void()>& pressedCallback);

	void update(InputHandler& inputHandler, float deltaTime) override;
	void render(UIRenderer* uiRenderer) override;

	void setCallback(const std::function<void()>& func);

	bool isDisabled() const;
	void setDisabled(bool disabled);
private:
	std::string text;
	bool isHovered;
	bool disabled;
	std::function<void()> callback;

	Font& minecraftFont;
	Texture& idleTexture;
	Texture& hoveredTexture;
	Texture& disabledTexture;

	Texture& getButtonTexture() const;
};
