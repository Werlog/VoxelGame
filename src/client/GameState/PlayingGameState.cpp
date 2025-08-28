#include "GameState/PlayingGameState.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include "UI/blockPickGUI.h"

PlayingGameState::PlayingGameState(Game* game, ResourceManager& resourceManager, const std::string& worldName, int worldSeed)
	: BaseGameState(game), terrainShader(resourceManager.getShader("shaders\\chunk")), minecraftFont(resourceManager.getFont("fonts\\MinecraftRegular.otf")), world(terrainShader, this, worldName, worldSeed), hud(game, this, &blockIcons),
	player(&(game->getCamera()), &world, &hud.getHotbar(), resourceManager), terrainSheet(16, 16, "textures/terrain.png"), blockIcons(resourceManager),
	skyboxShader(resourceManager.getShader("shaders\\skybox")), skybox(glm::vec3(0.0f, 0.3f, 1.0f), glm::vec3(0.7f, 0.9f, 1.0f), &skyboxShader), clouds(resourceManager)
{
	setupShader();

	currentGUI = nullptr;

	enableDevMenu = false;
	enableCollisionOption = true;
	enableFlightOption = false;
	paused = false;
}

void PlayingGameState::update(float deltaTime, InputHandler& inputHandler)
{
	if (currentGUI != nullptr)
	{
		currentGUI->update(inputHandler, deltaTime);
		checkNextGUI();
	}

	if (paused)
	{
		return;
	}
	if (enableDevMenu)
	{
		devMenuLogic(inputHandler);
		return;
	}

	hud.update(inputHandler, deltaTime);
	player.update(inputHandler, deltaTime);

	world.updateWorld(player);

	updateBreakParticles(deltaTime);

	clouds.update(deltaTime);
	devMenuLogic(inputHandler);

	if (inputHandler.getKeyDown(SDLK_ESCAPE))
	{
		setPaused(true);
		openGUI(std::make_shared<PauseMenuGUI>(game, this));
	}
	else if (inputHandler.getKeyDown(SDLK_e))
	{
		setPaused(true);

		std::shared_ptr<BlockPickGUI> gui = std::make_shared<BlockPickGUI>(game, this);
		gui->setBlockSelectCallback([this](BlockType type) {
			hud.getHotbar().pickBlock(type);
		});

		openGUI(gui);
	}
}

void PlayingGameState::render()
{
	Camera& camera = game->getCamera();

	skybox.render(camera);

	renderWorld();

	player.render();

	renderBreakParticles();

	clouds.render(&camera, player.getWorldPosition());

	if (currentGUI != nullptr)
	{
		currentGUI->render();
	}

	hud.render(&game->getUIRenderer());
}

void PlayingGameState::onEnter()
{
	world.updateLoadedChunks(ChunkCoord::toChunkCoord(player.getWorldPosition()));

	blockIcons.init(world.getBlockData());
}

void PlayingGameState::onExit()
{

}

void PlayingGameState::setPaused(bool paused)
{
	this->paused = paused;
	game->getCamera().resetFirstMouse();

	if (paused)
	{
		int width = game->getUIRenderer().getWindowWidth();
		int height = game->getUIRenderer().getWindowHeight();

		game->setMousePosition(width / 2, height / 2);
	}

	SDL_SetRelativeMouseMode(paused ? SDL_FALSE : SDL_TRUE);
}

void PlayingGameState::openGUI(std::shared_ptr<GUI> gui)
{
	this->currentGUI = gui;
}

void PlayingGameState::spawnBreakParticles(const glm::vec3& position, BlockType block)
{
	blockBreakParticles.emplace_back(block, world.getBlockData(), position, game->getResourceManager(), terrainSheet);
}

Player& PlayingGameState::getPlayer()
{
	return player;
}

World& PlayingGameState::getWorld()
{
	return world;
}

HUD& PlayingGameState::getHUD()
{
	return hud;
}

BlockIcons& PlayingGameState::getBlockIcons()
{
	return blockIcons;
}

void PlayingGameState::updateBreakParticles(float deltaTime)
{
	for (auto it = blockBreakParticles.begin(); it != blockBreakParticles.end();)
	{
		BreakParticles& particles = *it;
		
		particles.update(deltaTime, player.getChunkPosition(), world);

		if (particles.isFinished())
		{
			it = blockBreakParticles.erase(it);
			continue;
		}
		it++;
	}
}

void PlayingGameState::renderWorld()
{
	Camera& camera = game->getCamera();
	glUseProgram(terrainShader.getProgramHandle());

	glUniformMatrix4fv(shaderProjectionLoc, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));
	glUniformMatrix4fv(shaderViewLoc, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
	glUniform3fv(shaderCameraPositionLoc, 1, glm::value_ptr(camera.getPosition()));

	glBindTexture(GL_TEXTURE_2D_ARRAY, terrainSheet.getTextureHandle());

	world.renderWorld(player.getChunkPosition(), camera);

	glBindVertexArray(0);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void PlayingGameState::renderBreakParticles()
{
	Camera& camera = game->getCamera();

	for (size_t i = 0; i < blockBreakParticles.size(); i++)
	{
		BreakParticles& particles = blockBreakParticles[i];

		particles.render(camera);
	}
}

void PlayingGameState::setupShader()
{
	glUseProgram(terrainShader.getProgramHandle());

	shaderViewLoc = glGetUniformLocation(terrainShader.getProgramHandle(), "view");
	shaderProjectionLoc = glGetUniformLocation(terrainShader.getProgramHandle(), "projection");
	shaderModelLoc = glGetUniformLocation(terrainShader.getProgramHandle(), "model");
	shaderCameraPositionLoc = glGetUniformLocation(terrainShader.getProgramHandle(), "cameraPosition");
	shaderLightDirectionLoc = glGetUniformLocation(terrainShader.getProgramHandle(), "lightDirection");

	glm::vec3 lightDirection = glm::normalize(glm::vec3(0.5f, 1.0f, 1.0f));
	glUniform3fv(shaderLightDirectionLoc, 1, glm::value_ptr(lightDirection));
}

void PlayingGameState::devMenuLogic(InputHandler& inputHandler)
{
	if (inputHandler.getKeyDown(SDLK_RSHIFT))
	{
		enableDevMenu = !enableDevMenu;
		game->getCamera().resetFirstMouse();
		if (!enableDevMenu)
		{
			SDL_SetRelativeMouseMode(SDL_TRUE);
		}
	}

	if (enableDevMenu)
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
		ImGui::Begin("Developer Menu");

		ImGui::Checkbox("Enable Flight", &enableFlightOption);
		ImGui::Checkbox("Enable Collisions", &enableCollisionOption);

		ImGui::End();

		player.setEnableFlight(enableFlightOption);
		player.setEnableCollision(enableCollisionOption);
	}
}

void PlayingGameState::checkNextGUI()
{
	if (currentGUI->shouldGUIClose())
	{
		currentGUI = currentGUI->getNextGUI();
	}
}
