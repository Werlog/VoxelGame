#include "GameState/PlayingGameState.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"

PlayingGameState::PlayingGameState(Game* game, ResourceManager& resourceManager)
	: BaseGameState(game), terrainShader(resourceManager.getShader("shaders\\chunk")), minecraftFont(resourceManager.getFont("fonts\\MinecraftRegular.otf")), world(terrainShader),
	player(&(game->getCamera()), &world, resourceManager), terrainTexture(resourceManager.getTexture("textures\\terrain.png")), terrainSheet(16, 16, &terrainTexture),
	skyboxShader(resourceManager.getShader("shaders\\skybox")), skybox(glm::vec3(0.0f, 0.3f, 0.8f), glm::vec3(0.7f, 0.9f, 1.0f), &skyboxShader), clouds(resourceManager)
{
	setupShader();

	enableDevMenu = false;
	enableCollisionOption = true;
	enableFlightOption = false;

	game->getConnectionHandler().getDispatcher().subscribe(ServerToClient::S_ADD_PLAYER, [this](Packet& packet) {
		this->onReceiveAddPlayer(packet);
	});
}

void PlayingGameState::update(float deltaTime, InputHandler& inputHandler)
{
	if (!enableDevMenu)
		player.update(inputHandler, deltaTime);

	world.updateWorld(player);

	clouds.update(deltaTime);
	devMenuLogic(inputHandler);
}

void PlayingGameState::render()
{
	Camera& camera = game->getCamera();

	skybox.render(camera);

	glUseProgram(terrainShader.getProgramHandle());

	glUniformMatrix4fv(shaderProjectionLoc, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));
	glUniformMatrix4fv(shaderViewLoc, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
	glUniform3fv(shaderCameraPositionLoc, 1, glm::value_ptr(camera.position));

	glBindTexture(GL_TEXTURE_2D, terrainTexture.getTextureHandle());

	world.renderWorld(player.getChunkPosition(), camera.position);
	glBindVertexArray(0);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	player.render();

	for (auto it = players.begin(); it != players.end(); it++)
	{
		RemotePlayer& rmPlayer = it->second;
		if (rmPlayer.getId() == game->getConnectionHandler().getLocalClient()->getId())
		{
			continue;
		}
		
		it->second.render(player.getChunkPosition(), camera.position);
	}

	clouds.render(&camera, player.getWorldPosition());

	const BlockProperties& block = world.getBlockData().getBlockProperties(player.getSelectedBlock());

	int width = game->getUIRenderer().getWindowWidth();
	int height = game->getUIRenderer().getWindowHeight();

	game->getUIRenderer().renderCrosshair(width / 2, height / 2, 21.0f);
	game->getUIRenderer().renderText(minecraftFont, block.blockName + " selected", width / 2, 150, 0.9f, glm::vec3(1.0f), TextAlignment::ALIGN_CENTER);
}

void PlayingGameState::onEnter()
{
	glBindTexture(GL_TEXTURE_2D, terrainSheet.getSheet().getTextureHandle());

	world.updateLoadedChunks(ChunkCoord::toChunkCoord(player.getWorldPosition()));
}

void PlayingGameState::onExit()
{

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

	unsigned int unitXLoc = glGetUniformLocation(terrainShader.getProgramHandle(), "texUnitX");
	unsigned int unitYLoc = glGetUniformLocation(terrainShader.getProgramHandle(), "texUnitY");
	unsigned int atlasSizeX = glGetUniformLocation(terrainShader.getProgramHandle(), "atlasSizeX");
	unsigned int atlasSizeY = glGetUniformLocation(terrainShader.getProgramHandle(), "atlasSizeY");

	float oneX = terrainSheet.getOneUnitX();
	float oneY = terrainSheet.getOneUnitY();

	int atlasSize = 16;
	glUniform1fv(unitXLoc, 1, &oneX);
	glUniform1fv(unitYLoc, 1, &oneY);
	glUniform1iv(atlasSizeX, 1, &atlasSize);
	glUniform1iv(atlasSizeY, 1, &atlasSize);
}

void PlayingGameState::devMenuLogic(InputHandler& inputHandler)
{
	if (inputHandler.getKeyDown(SDLK_RSHIFT))
	{
		enableDevMenu = !enableDevMenu;

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

void PlayingGameState::onReceiveAddPlayer(Packet& packet)
{
	unsigned short clientId = packet.readUShort();
	std::string username = packet.readString();
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

	Client* client = game->getConnectionHandler().getClientById(clientId);

	auto inserted = players.try_emplace(clientId, client, game, username, position);
}
