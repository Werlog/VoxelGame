#pragma once

#include <glm/glm.hpp>
#include "camera.h"
#include "UI/uirenderer.h"
#include "chunk.h"
#include "texturesheet.h"

constexpr float viewModelFOV = 60;

class ViewModel
{
public:

	ViewModel(UIRenderer& uiRenderer, BlockData& blockData, TextureSheet& terrainSheet, ResourceManager& resourceManager);

	void update();
	void render(Camera& camera);

	void setViewModelBlockType(BlockType blockType);
private:
	UIRenderer& uiRenderer;
	BlockData& blockData;
	Shader& chunkShader;
	TextureSheet& terrainSheet;

	glm::mat4 projectionMatrix;
	glm::vec3 viewModelPos;

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

	void onResize();
};
