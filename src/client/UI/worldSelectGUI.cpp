#include "UI/worldSelectGUI.h"
#include "UI/elements/selectview.h"
#include "UI/elements/label.h"
#include "UI/elements/button.h"
#include "UI/mainMenuGUI.h"
#include "UI/createWorldGUI.h"
#include "saving/worldsaveutil.h"


WorldSelectGUI::WorldSelectGUI(Game* game, MainMenuGameState* mainMenu)
	: GUI(game)
{
	this->game = game;
	this->mainMenu = mainMenu;

	constructGUI();
}

void WorldSelectGUI::constructGUI()
{
	std::shared_ptr<SelectView> worldSelect = std::make_shared<SelectView>(game, glm::vec2(0.5f, 0.55f), glm::vec2(0.0f), glm::vec2(250.0f, 125.0f), saveutil::getSavedWorlds());

	std::shared_ptr<Label> label = std::make_shared<Label>(game, glm::vec2(0.5f, 0.55f), glm::vec2(-245.0f, 135.0f), glm::vec2(0.6f), "Select a world:", glm::vec3(0.8f), TextAlignment::ALIGN_LEFT);

	std::shared_ptr<Button> selectButton = std::make_shared<Button>(game, glm::vec2(0.5f, 0.06f), glm::vec2(0.0f, 150.0f), glm::vec2(300, 30), "Play selected world", [this, worldSelect] {
		if (worldSelect->getSelectedIndex() == -1)
		{
			std::cout << "No world selected" << std::endl;
			return;
		}
		mainMenu->enterSavedGame(worldSelect->getSelectedOption());
	});
	selectButton->setDisabled(true);

	worldSelect->setSelectionChangedCallback([selectButton](int selectedOption, const std::string& optionText) {
		selectButton->setDisabled(selectedOption == -1);
	});

	std::shared_ptr<Button> cancelButton = std::make_shared<Button>(game, glm::vec2(0.5f, 0.06f), glm::vec2(-155.0f, 80.0f), glm::vec2(145, 30), "Cancel", [this] {
		std::shared_ptr<MainMenuGUI> mainGUI = std::make_shared<MainMenuGUI>(game, mainMenu);
		mainMenu->switchToGUI(mainGUI);
	});

	std::shared_ptr<Button> newWorldButton = std::make_shared<Button>(game, glm::vec2(0.5f, 0.06f), glm::vec2(155.0f, 80.0f), glm::vec2(145, 30), "New world", [this] {
		std::shared_ptr<CreateWorldGUI> createWorldGUI = std::make_shared<CreateWorldGUI>(game, mainMenu);
		mainMenu->switchToGUI(createWorldGUI);
	});

	uiElements.push_back(worldSelect);
	uiElements.push_back(label);
	uiElements.push_back(selectButton);
	uiElements.push_back(newWorldButton);
	uiElements.push_back(cancelButton);
}
