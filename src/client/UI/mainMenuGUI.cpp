#pragma once

#include "UI/mainMenuGUI.h"
#include "UI/elements/button.h"
#include "UI/singleplayerGUI.h"

MainMenuGUI::MainMenuGUI(Game* game, MainMenuGameState* mainMenu) : GUI(game)
{
	this->mainMenu = mainMenu;
	constructGUI();
}

void MainMenuGUI::constructGUI()
{
	std::shared_ptr<Button> singleplayerButton = std::make_shared<Button>(game, glm::vec2(0.5f), glm::vec2(0, 40), glm::vec2(300, 30), "Play Singleplayer", [this]() {
		std::shared_ptr<SingleplayerGUI> gui = std::make_shared<SingleplayerGUI>(game, mainMenu);
		mainMenu->switchToGUI(gui);
	});

	std::shared_ptr<Button> quitButton = std::make_shared<Button>(game, glm::vec2(0.5f), glm::vec2(0, -40), glm::vec2(300, 30), "Quit Game", [this]() {
		game->quitGame();
	});

	uiElements.push_back(singleplayerButton);
	uiElements.push_back(quitButton);
}
