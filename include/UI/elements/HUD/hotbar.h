#pragma once

#include "../uielement.h"
#include "../../blockicons.h"
#include <array>
#include <functional>

class PlayingGameState;

class Hotbar : public UIElement
{
public:
	Hotbar(Game* game, PlayingGameState* playingState, glm::vec2 relPos, glm::vec2 offset, BlockIcons* blockIcons);

	void update(InputHandler& inputHandler, float deltaTime) override;
	void render(UIRenderer* uiRenderer) override;

	BlockType getSelectedBlock() const;
	void setBlocks(BlockType (&blocks)[9]);

	void pickBlock(BlockType type);
	void handleMiddleClick(BlockType type);

	void setBlockSelectCallback(const std::function<void(int, BlockType)>& callback);

	BlockType* getBlocks();
private:
	PlayingGameState* playingState;
	std::function<void(int, BlockType)> blockSelectCallback;
	float sinceSwitched;

	int selectedIndex;

	std::array<BlockType, 9> hotbarBlocks;

	Texture& hotbarTexture;
	Texture& selectedTexture;
	Font& minecraftFont;
	BlockIcons* blockIcons;

	void handleNumberKeys(InputHandler& inputHandler);

	void selectIndex(int index);
};
