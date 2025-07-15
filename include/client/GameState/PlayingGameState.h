#pragma once

#include "GameState/BaseGameState.h"
#include "world.h"
#include "resourcemanager.h"
#include "texturesheet.h"
#include "skybox.h"
#include "player.h"
#include "clouds.h"
#include "UI/pauseMenuGUI.h"
#include "UI/elements/HUD/HUD.h"
#include "UI/blockicons.h"

class PlayingGameState : public BaseGameState
{
public:
	PlayingGameState(Game* game, ResourceManager& resourceManager, const std::string& worldName, int worldSeed = 0);

	void update(float deltaTime, InputHandler& inputHandler) override;

	void render() override;

	void onEnter() override;
	void onExit() override;

	void setPaused(bool paused);

	void openGUI(std::shared_ptr<GUI> gui);

	Player& getPlayer();
	World& getWorld();
	HUD& getHUD();
	BlockIcons& getBlockIcons();

private:
	Shader& terrainShader;
	Font& minecraftFont;
	World world;
	HUD hud;
	Player player;
	TextureSheet terrainSheet;
	BlockIcons blockIcons;

	Shader& skyboxShader;
	SkyBox skybox;
	Clouds clouds;

	std::shared_ptr<GUI> currentGUI;

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
	void checkNextGUI();
};
