#include "GameState/MainMenuGameState.h"
#include "GameState/PlayingGameState.h"
#include "imgui.h"
#include <SDL2/SDL.h>
#include "saving/worldsaveutil.h"

MainMenuGameState::MainMenuGameState(Game* game) : BaseGameState(game),
	minecraftFont(game->getResourceManager().getFont("fonts\\MinecraftRegular.otf")),
	backgroundTexture(game->getResourceManager().getTexture("textures\\menuBackground.png")),
	titleTexture(game->getResourceManager().getTexture("textures\\title.png"))
{
	currentGUI = nullptr;

	this->currentGUI = std::make_shared<MainMenuGUI>(game, this);
}

void MainMenuGameState::update(float deltaTime, InputHandler& inputHandler)
{

	if (currentGUI != nullptr)
	{
		checkNextGUI();
		currentGUI->update(inputHandler, deltaTime);
	}
}

void MainMenuGameState::render()
{
	int width = game->getUIRenderer().getWindowWidth();
	int height = game->getUIRenderer().getWindowHeight();

	glm::vec2 halfScreen = glm::vec2(width / 2, height / 2);
	glm::vec2 uvScaler = glm::vec2(width / 60, height / 60);

	if (currentGUI != nullptr)
		currentGUI->render();

	game->getUIRenderer().renderTexturedQuad(titleTexture, glm::vec2(width / 2, height - 100), glm::vec2(343, 47), glm::vec2(1.0f));
	game->getUIRenderer().renderTexturedQuad(backgroundTexture, halfScreen, halfScreen, uvScaler);
}

void MainMenuGameState::onEnter()
{
	SDL_SetRelativeMouseMode(SDL_FALSE);
}

void MainMenuGameState::onExit()
{
	
}

void MainMenuGameState::enterGame(const std::string& seedText, const std::string& worldName)
{
	auto hashCode = [seedText]() -> int {
		int total = 0;
		for (size_t i = 0; i < seedText.size(); i++)
		{
			total += seedText[i] * pow(31, seedText.size() - (i + 1));
		}
		return total;
	};

	int seed = 0;
	if (seedText.size() > 0)
	{
		try
		{
			seed = std::stoi(seedText);
		}
		catch (...)
		{
			seed = hashCode();
		}
	}

	PlayingGameState* state = new PlayingGameState(game, game->getResourceManager(), worldName, seed);
	game->switchToState(state);
}

void MainMenuGameState::enterSavedGame(const std::string& worldName)
{
	SavedWorldInfo info = {};
	bool success = saveutil::getWorldInfo(worldName, &info);
	if (!success)
	{
		std::cout << "world.bin does not exist, cannot open world." << std::endl;
		return;
	}

	PlayingGameState* state = new PlayingGameState(game, game->getResourceManager(), worldName, info.worldSeed);
	state->getPlayer().setWorldPosition(glm::vec3(info.playerX, info.playerY, info.playerZ));
	state->getPlayer().setRotation(info.playerYaw, info.playerPitch);
	state->getPlayer().setHotbarItems(info.playerHotbar);
	game->switchToState(state);
}

void MainMenuGameState::checkNextGUI()
{
	if (currentGUI->shouldGUIClose())
	{
		currentGUI = currentGUI->getNextGUI();
	}
}
