#include "GameState/MainMenuGameState.h"
#include "GameState/PlayingGameState.h"
#include "imgui.h"
#include <SDL2/SDL.h>

MainMenuGameState::MainMenuGameState(Game* game) : BaseGameState(game),
	minecraftFont(game->getResourceManager().getFont("fonts\\MinecraftRegular.otf"))
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

	game->getUIRenderer().renderText(minecraftFont, "MAIN MENU", width / 2, height - 150, 2.0f, glm::vec3(1), TextAlignment::ALIGN_CENTER);
}

void MainMenuGameState::onEnter()
{
	glClearColor(0.1f, 0.5f, 1.0f, 0.5f);
	SDL_SetRelativeMouseMode(SDL_FALSE);
}

void MainMenuGameState::onExit()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	SDL_SetRelativeMouseMode(SDL_TRUE);
}
