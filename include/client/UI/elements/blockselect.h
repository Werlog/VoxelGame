#pragma once

#include "uielement.h"
#include "../blockicons.h"
#include <functional>

constexpr int blockSelectCellSize = 40;

class BlockSelect : public UIElement
{
public:

	BlockSelect(Game* game, glm::vec2 relPos, glm::vec2 offset, int rows, int columns, BlockIcons* blockIcons);

	void update(InputHandler& inputHandler, float deltaTime) override;
	void render(UIRenderer* uiRenderer) override;

	void setBlockSelectCallback(const std::function<void(BlockType)>& callback);
private:
	std::function<void(BlockType)> blockSelectFunc;

	int maxBlockId;

	int rows, columns;

	int hoveredBlock;

	BlockIcons* blockIcons;
};
