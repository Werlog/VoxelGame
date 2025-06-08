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

	std::shared_ptr<Button> resumeButton = std::make_shared<Button>(game, glm::vec2(0.5f), glm::vec2(0, 40), glm::vec2(300, 30), "Back to Game", [this]() {
		playingState->SetPaused(false);
	});

	std::shared_ptr<Button> quitButton = std::make_shared<Button>(game, glm::vec2(0.5f), glm::vec2(0, -40), glm::vec2(300, 30), "Quit Game", [this]() {
		game->quitGame();
	});

	uiElements.push_back(label);
	uiElements.push_back(resumeButton);
	uiElements.push_back(quitButton);
	uiElements.push_back(image);
}
