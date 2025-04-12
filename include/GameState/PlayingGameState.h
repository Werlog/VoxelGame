#pragma once

#include "GameState/BaseGameState.h"
#include "world.h"
#include "resourcemanager.h"
#include "texturesheet.h"
#include "skybox.h"
#include "player.h"

class PlayingGameState : public BaseGameState
{
public:
	PlayingGameState(Game* game, ResourceManager& resourceManager);

	void update(float deltaTime, InputHandler& inputHandler) override;

	void render() override;

	void onEnter() override;
	void onExit() override;
private:
	Shader& terrainShader;
	World world;
	Player player;
	Texture& terrainTexture;
	TextureSheet terrainSheet;

	Shader& skyboxShader;
	SkyBox skybox;

	unsigned int shaderViewLoc;
	unsigned int shaderProjectionLoc;
	unsigned int shaderModelLoc;

	void setupShader();
};
