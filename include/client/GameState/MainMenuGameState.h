#pragma once

#include "GameState/BaseGameState.h"

class MainMenuGameState : public BaseGameState
{
public:

	MainMenuGameState(Game* game);

	void update(float deltaTime, InputHandler& inputHandler) override;

	void render() override;

	void onEnter() override;
	void onExit() override;
private:
	Font& minecraftFont;

	Texture& backgroundTexture;
	Texture& titleTexture;

	char usernameText[128];
	char ipText[128];

	bool multiplayerMenuOpen;
};
