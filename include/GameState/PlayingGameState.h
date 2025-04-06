#pragma once

#include "GameState/BaseGameState.h"
#include "world.h"
#include "resourcemanager.h"
#include "texturesheet.h"

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
	Texture& terrainTexture;
	TextureSheet terrainSheet;

	unsigned int shaderViewLoc;
	unsigned int shaderProjectionLoc;
	unsigned int shaderModelLoc;

	void setupShader();
};
