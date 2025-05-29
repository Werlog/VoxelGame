#include "UI/uirenderer.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

UIRenderer::UIRenderer()
{
	defaultShader = nullptr;
	textShader = nullptr;

	textVAO = 0;
	textVBO = 0;

	crosshairVAO = 0;
	crosshairVBO = 0;
	crosshairEBO = 0;

	windowWidth = 0;
	windowHeight = 0;

	textProjectionLoc = 0;
	textColorLoc = 0;

	defaultProjectionLoc = 0;
	defaultModelLoc = 0;
}

void UIRenderer::init(ResourceManager& resourceManager, int width, int height)
{
	textShader = &resourceManager.getShader("shaders\\UI\\text");
	defaultShader = &resourceManager.getShader("shaders\\UI\\defaultUI");
	crosshairTexture = &resourceManager.getTexture("textures\\crosshair.png");

	setup();
	updateProjectionMatrix(width, height);
}

void UIRenderer::updateProjectionMatrix(int width, int height)
{
	uiProjection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);

	windowWidth = width;
	windowHeight = height;

	glUseProgram(textShader->getProgramHandle());

	glUniformMatrix4fv(textProjectionLoc, 1, GL_FALSE, glm::value_ptr(uiProjection));

	glUseProgram(defaultShader->getProgramHandle());
	glUniformMatrix4fv(defaultProjectionLoc, 1, GL_FALSE, glm::value_ptr(uiProjection));

	glUseProgram(0);
}

void UIRenderer::renderText(Font& font, const std::string& text, float x, float y, float scale, const glm::vec3& color, TextAlignment alignment)
{
	if (alignment != TextAlignment::ALIGN_LEFT)
	{
		float width = getTextWidth(font, text, scale);
		x -= alignment == TextAlignment::ALIGN_CENTER ? width * 0.5f : width;
	}

	glUseProgram(textShader->getProgramHandle());

	glUniform3fv(textColorLoc, 1, glm::value_ptr(color));

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(textVAO);

	for (auto it = text.begin(); it != text.end(); it++)
	{
		const Character& ch = font.getCharacter(*it);

		float xpos = x + ch.bearing.x * scale;
		float ypos = y - (ch.size.y - ch.bearing.y) * scale;

		float w = ch.size.x * scale;
		float h = ch.size.y * scale;
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};

		glBindTexture(GL_TEXTURE_2D, ch.texID);

		glBindBuffer(GL_ARRAY_BUFFER, textVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), &vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (ch.advance >> 6) * scale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void UIRenderer::renderCrosshair(float x, float y, float scale)
{
	glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(x, y, 0.0f));
	model = glm::scale(model, glm::vec3(scale));

	glUseProgram(defaultShader->getProgramHandle());
	glBindTexture(GL_TEXTURE_2D, crosshairTexture->getTextureHandle());

	glUniformMatrix4fv(defaultModelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(crosshairVAO);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

float UIRenderer::getTextWidth(Font& font, const std::string& text, float scale)
{
	float width = 0.0f;

	for (size_t i = 0; i < text.size(); i++)
	{
		const Character& ch = font.getCharacter(text[i]);

		if (i != text.size() - 1)
		{
			width += (ch.advance >> 6) * scale;
			continue;
		}
		width += ch.size.x * scale;
	}

	return width;
}

int UIRenderer::getWindowWidth()
{
	return windowWidth;
}

int UIRenderer::getWindowHeight()
{
	return windowHeight;
}

void UIRenderer::setup()
{
	// Setup Text
	glGenVertexArrays(1, &textVAO);
	glBindVertexArray(textVAO);

	glGenBuffers(1, &textVBO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	// Setup crosshair

	float vertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
	};

	int indices[] =
	{
		0, 1, 2,
		1, 3, 2,
	};

	glGenVertexArrays(1, &crosshairVAO);
	glBindVertexArray(crosshairVAO);

	glGenBuffers(1, &crosshairVBO);
	glGenBuffers(1, &crosshairEBO);

	glBindBuffer(GL_ARRAY_BUFFER, crosshairVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, crosshairEBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	// Setup shader uniforms

	glUseProgram(textShader->getProgramHandle());

	textProjectionLoc = glGetUniformLocation(textShader->getProgramHandle(), "projection");
	textColorLoc = glGetUniformLocation(textShader->getProgramHandle(), "textColor");

	defaultProjectionLoc = glGetUniformLocation(defaultShader->getProgramHandle(), "projection");
	defaultModelLoc = glGetUniformLocation(defaultShader->getProgramHandle(), "model");

	glUseProgram(0);
}
