#include "GameState/PlayingGameState.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

PlayingGameState::PlayingGameState(Game* game, ResourceManager& resourceManager)
	: BaseGameState(game), terrainShader(resourceManager.getShader("shaders\\chunk")), minecraftFont(resourceManager.getFont("fonts\\MinecraftRegular.otf")), world(terrainShader),
	player(&(game->getCamera()), &world, resourceManager), terrainTexture(resourceManager.getTexture("textures\\terrain.png")), terrainSheet(16, 16, &terrainTexture),
	skyboxShader(resourceManager.getShader("shaders\\skybox")), skybox(glm::vec3(0.0f, 0.45f, 1.0f), glm::vec3(0.3f, 0.9f, 1.0f), &skyboxShader)
{
	setupShader();
}

void PlayingGameState::update(float deltaTime, InputHandler& inputHandler)
{
	player.update(inputHandler, deltaTime);
	world.updateWorld(player);
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

	world.renderWorld();
	glBindVertexArray(0);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	player.render();

	const BlockProperties& block = world.getBlockData().getBlockProperties(player.getSelectedBlock());

	game->getUIRenderer().renderText(minecraftFont, block.blockName + " selected", game->getUIRenderer().getWindowWidth() / 2, 150, 0.9f, glm::vec3(1.0f), TextAlignment::ALIGN_CENTER);
}

void PlayingGameState::onEnter()
{
	glBindTexture(GL_TEXTURE_2D, terrainSheet.getSheet().getTextureHandle());

	world.updateLoadedChunks(ChunkCoord::toChunkCoord(player.getPosition()));
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
