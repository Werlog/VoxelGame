#include "viewmodel/viewmodel.h"
#include <glad/glad.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "viewmodel/motions/walkingmotion.h"
#include "viewmodel/motions/fallingmotion.h"
#include "viewmodel/motions/lookmotion.h"

ViewModel::ViewModel(UIRenderer& uiRenderer, BlockData& blockData, TextureSheet& terrainSheet, ResourceManager& resourceManager, Player& player)
	: uiRenderer(uiRenderer), blockData(blockData), terrainSheet(terrainSheet), chunkShader(resourceManager.getShader("shaders/chunk")), player(player)
{
	this->viewSSBO = 0;
	this->viewVAO = 0;

	this->FBO = 0;
	this->depthHandle = 0;
	this->viewTexture = 0;
	this->viewFaceCount = 0;
	this->viewModelPos = glm::vec3(1.25f, -1.67f, -2.75f);

	this->equipMotion = nullptr;

	init();

	setViewModelBlockType(BlockType::GRASS);
}

void ViewModel::update(float deltaTime, InputHandler& inputHandler)
{
	for (auto it = viewMotions.begin(); it != viewMotions.end(); it++)
	{
		const std::shared_ptr<ViewMotion>& motion = *it;
		motion->update(deltaTime, inputHandler, player);
	}

	if (uiRenderer.getWindowWidth() != prevWidth || uiRenderer.getWindowHeight() != prevHeight)
	{
		onResize();
	}
}

void ViewModel::render(Camera& camera)
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glm::vec3 swingRotation = swingMotion->getRotation();

	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), calculateDisplayPosition());
	model = glm::rotate(model, glm::radians(-40.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	model = glm::rotate(model, glm::radians(swingRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(swingRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(swingRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	glUseProgram(chunkShader.getProgramHandle());
	glBindTexture(GL_TEXTURE_2D_ARRAY, terrainSheet.getTextureHandle());

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3f(lightDirLoc, 0.0f, 1.0f, 1.0f);

	glBindVertexArray(viewVAO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, viewSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, blockData.getShapeSSBO());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, viewFaceCount * 6);

	glUseProgram(0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	float halfWidth = uiRenderer.getWindowWidth() / 2;
	float halfHeight = uiRenderer.getWindowHeight() / 2;
	uiRenderer.renderTexturedQuad(viewTexture, glm::vec2(halfWidth, halfHeight), glm::vec2(halfWidth, halfHeight), glm::vec2(1.0f));
}

void ViewModel::setViewModelBlockType(BlockType blockType)
{
	std::shared_ptr<Block> block = blockData.getBlock(blockType);
	const std::vector<BlockShapeFace>& faces = block->getBlockShape(blockType)->getFaces();

	std::vector<ChunkFace> blockFaces = std::vector<ChunkFace>();
	blockFaces.reserve(faces.size());

	for (size_t i = 0; i < faces.size(); i++)
	{
		const BlockShapeFace& face = faces[i];

		constexpr int lightLevel = 15;

		int shapeIndex = blockData.getShapeIndex(block->getBlockShape(blockType)->getShapeType());

		blockFaces.push_back(ChunkFace{ (face.textureId << 18 | lightLevel << 26), (shapeIndex | (int)i << 4) });
	}

	glBindVertexArray(viewVAO);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, viewSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, blockFaces.size() * sizeof(ChunkFace), blockFaces.data(), GL_DYNAMIC_DRAW);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	glBindVertexArray(0);

	viewFaceCount = blockFaces.size();
	equipMotion->itemSwitched();
}

void ViewModel::init()
{
	glGenVertexArrays(1, &viewVAO);
	glGenBuffers(1, &viewSSBO);

	onResize();
	initUniforms();
	initMotions();
}

void ViewModel::initUniforms()
{
	viewLoc = glGetUniformLocation(chunkShader.getProgramHandle(), "view");
	modelLoc = glGetUniformLocation(chunkShader.getProgramHandle(), "model");
	projectionLoc = glGetUniformLocation(chunkShader.getProgramHandle(), "projection");
	lightDirLoc = glGetUniformLocation(chunkShader.getProgramHandle(), "lightDirection");
}

void ViewModel::initMotions()
{
	equipMotion = std::make_shared<EquipMotion>();
	swingMotion = std::make_shared<SwingMotion>();

	viewMotions.push_back(equipMotion);
	viewMotions.push_back(swingMotion);
	viewMotions.push_back(std::make_shared<WalkingMotion>());
	viewMotions.push_back(std::make_shared<FallingMotion>());
	viewMotions.push_back(std::make_shared<LookMotion>());
}

glm::vec3 ViewModel::calculateDisplayPosition()
{
	glm::vec3 pos = viewModelPos;

	for (auto it = viewMotions.begin(); it != viewMotions.end(); it++)
	{
		const std::shared_ptr<ViewMotion>& motion = *it;
		pos += motion->getOffset() * motion->getWeight();
	}

	return pos;
}

void ViewModel::onResize()
{
	float aspect = uiRenderer.getWindowWidth() / (float)uiRenderer.getWindowHeight();
	projectionMatrix = glm::perspective(glm::radians(viewModelFOV), aspect, nearPlane, farPlane);

	if (depthHandle != 0)
		glDeleteTextures(1, &depthHandle);
	if (viewTexture != 0)
		glDeleteTextures(1, &viewTexture);
	if (FBO == 0)
		glGenFramebuffers(1, &FBO);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &viewTexture);
	glBindTexture(GL_TEXTURE_2D, viewTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, uiRenderer.getWindowWidth(), uiRenderer.getWindowHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, viewTexture, 0);

	glGenTextures(1, &depthHandle);
	glBindTexture(GL_TEXTURE_2D, depthHandle);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, uiRenderer.getWindowWidth(), uiRenderer.getWindowHeight());
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthHandle, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "FAILED TO LOAD FRAMEBUFFER FOR VIEW-MODEL" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	this->prevWidth = uiRenderer.getWindowWidth();
	this->prevHeight = uiRenderer.getWindowHeight();
}
