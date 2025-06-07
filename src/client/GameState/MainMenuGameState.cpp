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
	singleplayerMenuOpen = false;

	std::memset(seedText, 0, sizeof(seedText));
}

void MainMenuGameState::update(float deltaTime, InputHandler& inputHandler)
{
	if (singleplayerMenuOpen)
	{
		ImGui::Begin("Play Singleplayer");

		ImGui::Text("You can either enter a world seed below");
		ImGui::Text("or leave it blank for a random seed.");

		ImGui::InputText("World Seed", seedText, IM_ARRAYSIZE(seedText));

		if (ImGui::Button("Create world"))
		{
			int seed = 0;
			if (seedText[0] != 0) // Input field is not empty
			{
				try
				{
					seed = std::stoi(std::string(seedText));
				}
				catch (...)
				{
					// Don't care, random seed
				}
			}

			PlayingGameState* playingState = new PlayingGameState(game, game->getResourceManager(), seed);
			game->switchToState(playingState);
		}
		if (ImGui::Button("Back"))
		{
			singleplayerMenuOpen = false;
		}

		ImGui::End();
	}
	else if (multiplayerMenuOpen)
	{
		ImGui::Begin("Play Multiplayer");
		ImGui::Text("Multiplayer has not been implemented yet.");
		if (ImGui::Button("Back"))
		{
			multiplayerMenuOpen = false;
		}

		ImGui::End();
	}
	else
	{
		ImGui::Begin("Main Menu Options");

		ImGui::Text("This is a temporary main menu");

		if (ImGui::Button("Play singleplayer"))
		{
			singleplayerMenuOpen = true;
		}
		if (ImGui::Button("Play multiplayer"))
		{
			multiplayerMenuOpen = true;
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
