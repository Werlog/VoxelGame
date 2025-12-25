#pragma once

#include <glm/glm.hpp>
#include "camera.h"
#include "UI/uirenderer.h"
#include "chunk.h"
#include "texturesheet.h"
#include "player.h"
#include "motions/equipmotion.h"
#include <vector>

constexpr float viewModelFOV = 60;

class ViewModel
{
public:

	ViewModel(UIRenderer& uiRenderer, BlockData& blockData, TextureSheet& terrainSheet, ResourceManager& resourceManager, Player& player);

	void update(float deltaTime, InputHandler& inputHandler);
	void render(Camera& camera);

	void setViewModelBlockType(BlockType blockType);
private:
	std::vector<std::shared_ptr<ViewMotion>> viewMotions;
	std::shared_ptr<EquipMotion> equipMotion;

	UIRenderer& uiRenderer;
	BlockData& blockData;
	Shader& chunkShader;
	TextureSheet& terrainSheet;

	Player& player;

	glm::mat4 projectionMatrix;
	glm::vec3 viewModelPos;

	int prevWidth;
	int prevHeight;

	unsigned int viewVAO;
	unsigned int viewSSBO;
	int viewFaceCount;

	unsigned int FBO;
	unsigned int depthHandle;
	unsigned int viewTexture;

	unsigned int viewLoc;
	unsigned int modelLoc;
	unsigned int projectionLoc;
	unsigned int lightDirLoc;

	void init();
	void initUniforms();
	void initMotions();

	glm::vec3 calculateDisplayPosition();

	void onResize();
};
