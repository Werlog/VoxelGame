#include "UI/singleplayerGUI.h"
#include "UI/elements/button.h"
#include "UI/elements/label.h"
#include "UI/elements/inputfield.h"
#include "UI/mainMenuGUI.h"
#include "GameState/MainMenuGameState.h"

SingleplayerGUI::SingleplayerGUI(Game* game, MainMenuGameState* mainMenu) : GUI(game)
{
	this->game = game;
	this->mainMenu = mainMenu;

	constructGUI();
}

void SingleplayerGUI::constructGUI()
{
	std::shared_ptr<Button> backButton = std::make_shared<Button>(game, glm::vec2(0.5f, 0.05f), glm::vec2(0.0f, 80.0f), glm::vec2(300, 30), "Cancel", [this]() {
		std::shared_ptr<MainMenuGUI> gui = std::make_shared<MainMenuGUI>(game, mainMenu);
		mainMenu->switchToGUI(gui);
	});

	std::shared_ptr<InputField> inputField = std::make_shared<InputField>(game, glm::vec2(0.5f), glm::vec2(0.0f), glm::vec2(300, 30));
	std::shared_ptr<Label> inputLabel = std::make_shared<Label>(game, glm::vec2(0.5f), glm::vec2(-295.0f, 40), glm::vec2(0.6f), "World Seed:", glm::vec3(0.8f), TextAlignment::ALIGN_LEFT);
	std::shared_ptr<Label> secondLabel = std::make_shared<Label>(game, glm::vec2(0.5f), glm::vec2(-295.0f, -60), glm::vec2(0.6f), "Leave blank for a random seed", glm::vec3(0.8f), TextAlignment::ALIGN_LEFT);

	std::shared_ptr<Button> button = std::make_shared<Button>(game, glm::vec2(0.5f, 0.05f), glm::vec2(0.0f, 150.0f), glm::vec2(300, 30), "Create world", [this, inputField]() {
		mainMenu->enterGame(inputField->getText());
	});

	uiElements.push_back(button);
	uiElements.push_back(backButton);
	uiElements.push_back(inputField);
	uiElements.push_back(inputLabel);
	uiElements.push_back(secondLabel);
}
