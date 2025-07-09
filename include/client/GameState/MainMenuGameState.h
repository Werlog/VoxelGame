#pragma once

#include "GameState/BaseGameState.h"
#include "UI/mainMenuGUI.h"

class MainMenuGameState : public BaseGameState
{
public:

	MainMenuGameState(Game* game);

	void update(float deltaTime, InputHandler& inputHandler) override;

	void render() override;

	void onEnter() override;
	void onExit() override;

	void switchToGUI(std::shared_ptr<GUI> newGUI);

	void enterGame(const std::string& seedText, const std::string& worldName);
	void enterSavedGame(const std::string& worldName);
private:
	Font& minecraftFont;

	std::shared_ptr<GUI> currentGUI;
	std::shared_ptr<GUI> nextGUI;

	Texture& backgroundTexture;
	Texture& titleTexture;

	bool multiplayerMenuOpen;
	bool singleplayerMenuOpen;

	void checkNextGUI();
};
