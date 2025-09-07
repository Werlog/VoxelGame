#include "UI/pauseMenuGUI.h"
#include "UI/elements/button.h"
#include "UI/elements/label.h"
#include "UI/elements/image.h"
#include "GameState/PlayingGameState.h"

PauseMenuGUI::PauseMenuGUI(Game* game, PlayingGameState* playingState) : GUI(game)
{
	this->playingState = playingState;
	constructGUI();
}

void PauseMenuGUI::constructGUI()
{
	std::shared_ptr<Image> image = std::make_shared<Image>(game, glm::vec2(0.5f), glm::vec2(0.0f), glm::vec2(0.0f), nullptr, glm::vec4(0.0f, 0.0f, 0.0f, 0.4f), true);

	std::shared_ptr<Label> label = std::make_shared<Label>(game, glm::vec2(0.5f, 1.0f), glm::vec2(0, -150), glm::vec2(0.75f), "Game Paused", glm::vec3(1.0f), TextAlignment::ALIGN_CENTER);
	std::shared_ptr<Label> savedLabel = std::make_shared<Label>(game, glm::vec2(0.5f), glm::vec2(155.0f, -110.0f), glm::vec2(0.6f), "", glm::vec3(1.0f), TextAlignment::ALIGN_CENTER);

	std::shared_ptr<Button> resumeButton = std::make_shared<Button>(game, glm::vec2(0.5f), glm::vec2(0, 40.0f), glm::vec2(300, 30), "Back to Game", [this]() {
		playingState->setPaused(false);
		closeGUI();
	});

	std::shared_ptr<Button> saveButton = std::make_shared<Button>(game, glm::vec2(0.5f), glm::vec2(155.0f, -40.0f), glm::vec2(145, 30), "Save world", [this, savedLabel]() {
		playingState->getWorld().saveWorld();
		savedLabel->setText("World saved!");
	});

	std::shared_ptr<Button> quitButton = std::make_shared<Button>(game, glm::vec2(0.5f), glm::vec2(-155.0f, -40.0f), glm::vec2(145, 30), "Quit Game", [this]() {
		game->quitGame();
	});

	uiElements.push_back(label);
	uiElements.push_back(savedLabel);
	uiElements.push_back(resumeButton);
	uiElements.push_back(saveButton);
	uiElements.push_back(quitButton);
	uiElements.push_back(image);
}
