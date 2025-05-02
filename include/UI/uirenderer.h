#pragma once

#include "shader.h"
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

	float getTextWidth(Font& font, const std::string& text, float scale);

	int getWindowWidth();
	int getWindowHeight();
private:
	Shader* textShader;

	int windowWidth;
	int windowHeight;

	unsigned int textVAO;
	unsigned int textVBO;

	unsigned int textProjectionLoc;
	unsigned int textColorLoc;

	glm::mat4 uiProjection;

	void setup();
};
