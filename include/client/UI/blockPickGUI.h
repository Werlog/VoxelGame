#pragma once

#include "gui.h"
#include "elements/blockselect.h"

class PlayingGameState;

class BlockPickGUI : public GUI
{
public:

	BlockPickGUI(Game* game, PlayingGameState* playingState);

	void setBlockSelectCallback(const std::function<void(BlockType)>& func);

protected:
	PlayingGameState* playingState;
	std::shared_ptr<BlockSelect> blockSelect;

	void constructGUI() override;
};
