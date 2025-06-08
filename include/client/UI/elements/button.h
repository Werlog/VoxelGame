#include "uielement.h"
#include "texture.h"
#include <string>
#include <functional>

class Button : public UIElement
{
public:

	Button(Game* game, glm::vec2 relPos, glm::vec2 offset, glm::vec2 size, const std::string& buttonText, std::function<void()> pressedCallback);

	void update(InputHandler& inputHandler) override;
	void render(UIRenderer* uiRenderer) override;
private:
	std::string text;
	bool isHovered;
	std::function<void()> callback;

	Font& minecraftFont;
	Texture& idleTexture;
	Texture& hoveredTexture;
};
