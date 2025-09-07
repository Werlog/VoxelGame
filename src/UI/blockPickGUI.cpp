#include "UI/blockPickGUI.h"
#include "GameState/PlayingGameState.h"
#include "UI/elements/button.h"
#include "UI/elements/label.h"

BlockPickGUI::BlockPickGUI(Game* game, PlayingGameState* playingState)
	: GUI(game)
{
	this->game = game;
	this->playingState = playingState;
	constructGUI();
}

void BlockPickGUI::setBlockSelectCallback(const std::function<void(BlockType)>& func)
{
	blockSelect->setBlockSelectCallback(func);
}

void BlockPickGUI::constructGUI()
{
	constexpr int rows = 4;
	constexpr int columns = 8;

	blockSelect = std::make_shared<BlockSelect>(game, glm::vec2(0.5f), glm::vec2(0.0f), rows, columns, &playingState->getBlockIcons());

	std::shared_ptr<Label> label = std::make_shared<Label>(game, glm::vec2(0.5f), glm::vec2(-columns * blockSelectCellSize, rows * blockSelectCellSize + 15.0f), glm::vec2(0.7f), "Block Picker", glm::vec3(0.9f), TextAlignment::ALIGN_LEFT);

	std::shared_ptr<Button> okButton = std::make_shared<Button>(game, glm::vec2(0.5f), glm::vec2(0.0f, -(rows * blockSelectCellSize) - 50.0f), glm::vec2(300, 30), "OK", [this]() {
		playingState->setPaused(false);
		closeGUI();
	});


	uiElements.push_back(label);
	uiElements.push_back(blockSelect);
	uiElements.push_back(okButton);
}