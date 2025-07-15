#pragma once

#include "../uielement.h"
#include "../../blockicons.h"
#include <array>

class PlayingGameState;

class Hotbar : public UIElement
{
public:
	Hotbar(Game* game, PlayingGameState* playingState, glm::vec2 relPos, glm::vec2 offset, BlockIcons* blockIcons);

	void update(InputHandler& inputHandler, float deltaTime) override;
	void render(UIRenderer* uiRenderer) override;

	BlockType getSelectedBlock() const;

	void pickBlock(BlockType type);
private:
	PlayingGameState* playingState;
	float sinceSwitched;

	int selectedIndex;

	std::array<BlockType, 9> hotbarBlocks;

	Texture& hotbarTexture;
	Texture& selectedTexture;
	Font& minecraftFont;
	BlockIcons* blockIcons;
};
