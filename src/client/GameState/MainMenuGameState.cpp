#include "GameState/MainMenuGameState.h"
#include "GameState/PlayingGameState.h"
#include "imgui.h"
#include <SDL2/SDL.h>

MainMenuGameState::MainMenuGameState(Game* game) : BaseGameState(game),
	minecraftFont(game->getResourceManager().getFont("fonts\\MinecraftRegular.otf")),
	backgroundTexture(game->getResourceManager().getTexture("textures\\menuBackground.png")),
	titleTexture(game->getResourceManager().getTexture("textures\\title.png"))
{
	multiplayerMenuOpen = false;

	std::memset(usernameText, 0, sizeof(usernameText));
	std::memset(ipText, 0, sizeof(ipText));
}

void MainMenuGameState::update(float deltaTime, InputHandler& inputHandler)
{
	if (!multiplayerMenuOpen)
	{
		ImGui::Begin("Main Menu Options");

		ImGui::Text("This is a temporary main menu");

		if (ImGui::Button("Play singleplayer"))
		{
			PlayingGameState* playingState = new PlayingGameState(game, game->getResourceManager());
			game->switchToState(playingState);
		}
		if (ImGui::Button("Play multiplayer"))
		{
			multiplayerMenuOpen = true;
		}

		ImGui::End();
	}
	else
	{
		ImGui::Begin("Play Multiplayer");
		ImGui::InputText("Username", usernameText, IM_ARRAYSIZE(usernameText));
		ImGui::InputText("IP Address", ipText, IM_ARRAYSIZE(ipText));
		
		if (ImGui::Button("Connect to server"))
		{
			std::cout << "Not implemented yet" << std::endl;
		}
		if (ImGui::Button("Back"))
		{
			multiplayerMenuOpen = false;
		}

		ImGui::End();
	}
}

void MainMenuGameState::render()
{
	int width = game->getUIRenderer().getWindowWidth();
	int height = game->getUIRenderer().getWindowHeight();

	glm::vec2 halfScreen = glm::vec2(width / 2, height / 2);
	glm::vec2 uvScaler = glm::vec2(width / 60, height / 60);

	game->getUIRenderer().renderTexturedQuad(titleTexture, glm::vec2(width / 2, height - 100), glm::vec2(343, 47), glm::vec2(1.0f));
	game->getUIRenderer().renderTexturedQuad(backgroundTexture, halfScreen, halfScreen, uvScaler);
}

void MainMenuGameState::onEnter()
{
	SDL_SetRelativeMouseMode(SDL_FALSE);
}

void MainMenuGameState::onExit()
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
}
