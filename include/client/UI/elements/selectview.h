#pragma once

#include "uielement.h"
#include <vector>
#include <initializer_list>
#include <functional>

constexpr int selectViewItemCount = 5;
constexpr float selectViewMaxFontScale = 0.7f;

class SelectView : public UIElement
{
public:

	SelectView(Game* game, glm::vec2 relPos, glm::vec2 offset, glm::vec2 size, const std::vector<std::string>& options);
	SelectView(Game* game, glm::vec2 relPos, glm::vec2 offset, glm::vec2 size, std::initializer_list<const char*> options);

	void update(InputHandler& inputHandler, float deltaTime) override;
	void render(UIRenderer* uiRenderer) override;

	void setSelectionChangedCallback(const std::function<void(int, const std::string&)>& func);

	int getSelectedIndex() const;
	std::string getSelectedOption() const;
private:
	std::vector<std::string> options;
	std::function<void(int, const std::string&)> selectionChangedCallback;

	Font& minecraftFont;

	size_t scrollIndex;
	float segmentHeight;
	float fontScale;

	int selectedIndex;
	int hoveredIndex;

	void renderScrollbar(UIRenderer* uiRenderer);
	void renderOptions(UIRenderer* uiRenderer);

	int getHoveredIndex(InputHandler& inputHandler);

	void init();

	bool shouldShowScrollbar();
};
