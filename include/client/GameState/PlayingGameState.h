#pragma once

#include "GameState/BaseGameState.h"
#include "world.h"
#include "resourcemanager.h"
#include "texturesheet.h"
#include "skybox.h"
#include "player.h"
#include "clouds.h"
#include "UI/pauseMenuGUI.h"

class PlayingGameState : public BaseGameState
{
public:
	PlayingGameState(Game* game, ResourceManager& resourceManager, const std::string& worldName, int worldSeed = 0);

	void update(float deltaTime, InputHandler& inputHandler) override;

	void render() override;

	void onEnter() override;
	void onExit() override;

	void SetPaused(bool paused);

	Player& getPlayer();
	World& getWorld();
private:
	Shader& terrainShader;
	Font& minecraftFont;
	World world;
	Player player;
	TextureSheet terrainSheet;

	Shader& skyboxShader;
	SkyBox skybox;
	Clouds clouds;

	PauseMenuGUI pauseGUI;

	bool enableDevMenu;
	bool enableCollisionOption;
	bool enableFlightOption;
	bool paused;

	unsigned int shaderViewLoc;
	unsigned int shaderProjectionLoc;
	unsigned int shaderModelLoc;
	unsigned int shaderCameraPositionLoc;
	unsigned int shaderLightDirectionLoc;

	void setupShader();
	void devMenuLogic(InputHandler& inputHandler);
};
