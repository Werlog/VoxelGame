#include "GameState/MainMenuGameState.h"
#include "GameState/PlayingGameState.h"
#include "imgui.h"
#include <SDL2/SDL.h>

MainMenuGameState::MainMenuGameState(Game* game) : BaseGameState(game),
	minecraftFont(game->getResourceManager().getFont("fonts\\MinecraftRegular.otf"))
{

}

void MainMenuGameState::update(float deltaTime, InputHandler& inputHandler)
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
		std::cout << "Multiplayer has not been implemented yet!" << std::endl;
	}

	ImGui::End();
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
