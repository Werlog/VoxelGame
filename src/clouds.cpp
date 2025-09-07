#include "clouds.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Clouds::Clouds(ResourceManager& resourceManager)
{
	cloudTexture = &resourceManager.getTexture("textures\\clouds.png");
	cloudShader = &resourceManager.getShader("shaders\\clouds");
	timeOffset = 0.0f;

	// Mesh setup
	float vertices[] =
	{
		-cloudPlaneSize, 0.0f, -cloudPlaneSize, 0.0f, 0.0f,
		cloudPlaneSize, 0.0f, -cloudPlaneSize, 1.0f, 0.0f,
		-cloudPlaneSize, 0.0f, cloudPlaneSize, 0.0f, 1.0f,
		cloudPlaneSize, 0.0f, cloudPlaneSize, 1.0f, 1.0f,
	};

	int indices[] =
	{
		0, 1, 2,
		1, 3, 2,
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	// Shader setup

	glUseProgram(cloudShader->getProgramHandle());

	viewLoc = glGetUniformLocation(cloudShader->getProgramHandle(), "view");
	projectionLoc = glGetUniformLocation(cloudShader->getProgramHandle(), "projection");
	playerPosLoc = glGetUniformLocation(cloudShader->getProgramHandle(), "playerPosition");
	planeSizeLoc = glGetUniformLocation(cloudShader->getProgramHandle(), "planeSize");
	timeLoc = glGetUniformLocation(cloudShader->getProgramHandle(), "time");

	glUniform1f(planeSizeLoc, cloudPlaneSize * 2.0f);

	glUseProgram(0);
}

void Clouds::update(float deltaTime)
{
	timeOffset += deltaTime;
}

void Clouds::render(Camera* camera, const glm::vec3& playerPosition)
{
	glDisable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D, cloudTexture->getTextureHandle());

	glUseProgram(cloudShader->getProgramHandle());

	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
	glUniform3fv(playerPosLoc, 1, glm::value_ptr(playerPosition));
	glUniform1f(timeLoc, timeOffset);

	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_CULL_FACE);
}
