#pragma once

#include "shader.h"
#include "texture.h"
#include "resourcemanager.h"
#include "UI/font.h"
#include <glm/glm.hpp>

enum TextAlignment : unsigned char
{
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT,
};

class UIRenderer
{
public:
	
	UIRenderer();

	void init(ResourceManager& resourceManager, int width, int height);

	void updateProjectionMatrix(int width, int height);

	void renderText(Font& font, const std::string& text, float x, float y, float scale, const glm::vec3& color, TextAlignment alignment = TextAlignment::ALIGN_LEFT);

	void renderCrosshair(float x, float y, float scale);

	void renderTexturedQuad(Texture& texture, glm::vec2 position, glm::vec2 scale, glm::vec2 uvMultiplier);

	void renderTexturedQuad(unsigned int textureHandle, glm::vec2 position, glm::vec2 scale, glm::vec2 uvMultiplier);

	void renderColoredQuad(const glm::vec4& color, glm::vec2 position, glm::vec2 scale);

	float getTextWidth(Font& font, const std::string& text, float scale);

	int getWindowWidth();
	int getWindowHeight();
private:
	Shader* textShader;
	Shader* defaultShader;

	Texture* crosshairTexture;

	int windowWidth;
	int windowHeight;

	unsigned int textVAO;
	unsigned int textVBO;

	unsigned int quadVAO;
	unsigned int quadVBO;
	unsigned int quadEBO;

	unsigned int textProjectionLoc;
	unsigned int textColorLoc;

	unsigned int defaultProjectionLoc;
	unsigned int defaultModelLoc;
	unsigned int defaultUvScalerLoc;
	unsigned int defaultTexAssignedLoc;
	unsigned int defaultColorLoc;

	glm::mat4 uiProjection;

	void setup();
};
