#pragma once

#include "GameState/BaseGameState.h"
#include "world.h"
#include "resourcemanager.h"
#include "texturesheet.h"
#include "skybox.h"
#include "player.h"
#include "clouds.h"
#include "remoteplayer.h"
#include "packet.h"

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
	Font& minecraftFont;
	World world;
	Player player;
	Texture& terrainTexture;
	TextureSheet terrainSheet;

	std::unordered_map<unsigned short, RemotePlayer> players;

	Shader& skyboxShader;
	SkyBox skybox;
	Clouds clouds;

	bool enableDevMenu;
	bool enableCollisionOption;
	bool enableFlightOption;

	unsigned int shaderViewLoc;
	unsigned int shaderProjectionLoc;
	unsigned int shaderModelLoc;
	unsigned int shaderCameraPositionLoc;
	unsigned int shaderLightDirectionLoc;

	void setupShader();
	void devMenuLogic(InputHandler& inputHandler);
	void onReceiveAddPlayer(Packet& packet);
};
