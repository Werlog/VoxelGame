#include "UI/createWorldGUI.h"
#include "UI/elements/button.h"
#include "UI/elements/label.h"
#include "UI/elements/inputfield.h"
#include "UI/worldSelectGUI.h"
#include "GameState/MainMenuGameState.h"
#include <filesystem>

CreateWorldGUI::CreateWorldGUI(Game* game, MainMenuGameState* mainMenu) : GUI(game)
{
	this->game = game;
	this->mainMenu = mainMenu;

	constructGUI();
}

void CreateWorldGUI::constructGUI()
{
	std::shared_ptr<Button> backButton = std::make_shared<Button>(game, glm::vec2(0.5f, 0.05f), glm::vec2(0.0f, 80.0f), glm::vec2(300, 30), "Cancel", [this]() {
		std::shared_ptr<WorldSelectGUI> gui = std::make_shared<WorldSelectGUI>(game, mainMenu);
		mainMenu->switchToGUI(gui);
	});

	std::shared_ptr<InputField> inputField = std::make_shared<InputField>(game, glm::vec2(0.5f), glm::vec2(0.0f), glm::vec2(300, 30));
	std::shared_ptr<Label> inputLabel = std::make_shared<Label>(game, glm::vec2(0.5f), glm::vec2(-295.0f, 40), glm::vec2(0.6f), "World Seed:", glm::vec3(0.8f), TextAlignment::ALIGN_LEFT);
	std::shared_ptr<Label> secondLabel = std::make_shared<Label>(game, glm::vec2(0.5f), glm::vec2(-295.0f, -60), glm::vec2(0.6f), "Leave blank for a random seed", glm::vec3(0.8f), TextAlignment::ALIGN_LEFT);

	std::shared_ptr<InputField> nameField = std::make_shared<InputField>(game, glm::vec2(0.5f), glm::vec2(0.0f, 110.0f), glm::vec2(300, 30));
	std::shared_ptr<Label> nameLabel = std::make_shared<Label>(game, glm::vec2(0.5f), glm::vec2(-295.0f, 150.0f), glm::vec2(0.6f), "World Name:", glm::vec3(0.8f), TextAlignment::ALIGN_LEFT);

	std::shared_ptr<Button> createButton = std::make_shared<Button>(game, glm::vec2(0.5f, 0.05f), glm::vec2(0.0f, 150.0f), glm::vec2(300, 30), "Create world", [this, inputField, nameField]() {
		std::string worldName = nameField->getText();
		if (!checkWorldName(worldName))
		{
			// TODO: Inform user that the name is invalid
			return;
		}

		mainMenu->enterGame(inputField->getText(), nameField->getText());
	});
	createButton->setDisabled(true);

	nameField->setOnChangedCallback([this, createButton](const std::string& fieldText) {
		createButton->setDisabled(!checkWorldName(fieldText));
	});

	uiElements.push_back(createButton);
	uiElements.push_back(backButton);
	uiElements.push_back(nameLabel);
	uiElements.push_back(nameField);
	uiElements.push_back(inputField);
	uiElements.push_back(inputLabel);
	uiElements.push_back(secondLabel);
}

bool CreateWorldGUI::checkWorldName(const std::string& worldName)
{
	if (worldName.empty()) return false;

	// Disallowed characters in a path string
	constexpr char disallowed[] = { '/', '\\', '<', '>', ':', '"', '|', '?', '*' };

	for (const auto& c : disallowed)
	{
		if (worldName.find(c) != std::string::npos)
			return false;
	}

	if (std::filesystem::is_directory(RESOURCES_PATH "saves/" + worldName))
	{
		return false;
	}

	return true;
}
