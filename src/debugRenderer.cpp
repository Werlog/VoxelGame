#include "debugRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

DebugRenderer::DebugRenderer()
{
	boxVAO = 0;
	boxVBO = 0;
	boxEBO = 0;

	boxViewLoc = 0;
	boxModelLoc = 0;
	boxProjectionLoc = 0;
	boxColorLoc = 0;
}

void DebugRenderer::init(ResourceManager& resourceManager, Camera* camera)
{
	this->camera = camera;
	setupBox(resourceManager);
}

void DebugRenderer::renderBox(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color)
{
	glUseProgram(boxShader->getProgramHandle());

	glm::mat4 model = glm::translate(glm::mat4(1.0f), start);
	model = glm::scale(model, (end - start));

	glUniformMatrix4fv(boxProjectionLoc, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));
	glUniformMatrix4fv(boxViewLoc, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
	glUniformMatrix4fv(boxModelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(boxColorLoc, 1, glm::value_ptr(color));

	glBindVertexArray(boxVAO);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
}

void DebugRenderer::setupBox(ResourceManager& resourceManager)
{
	boxShader = &resourceManager.getShader("shaders\\debug\\debugBox");

	float vertices[] =
	{
		-0.00f, -0.00f, -0.00f,
		1.00f, -0.00f, -0.00f,
		-0.00f, 1.00f, -0.00f,
		1.00f, 1.00f, -0.00f,

		-0.00f, -0.00f, 1.00f,
		1.00f, -0.00f, 1.00f,
		-0.00f, 1.00f, 1.00f,
		1.00f, 1.00f, 1.00f,
	};

	unsigned int indices[] =
	{
		0, 1,
		0, 2,
		2, 3,
		1, 3,

		4, 5,
		4, 6,
		6, 7,
		5, 7,

		0, 4,
		1, 5,
		2, 6,
		3, 7,
	};

	glGenVertexArrays(1, &boxVAO);
	glBindVertexArray(boxVAO);

	glGenBuffers(1, &boxVBO);
	glGenBuffers(1, &boxEBO);

	glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boxEBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	glUseProgram(boxShader->getProgramHandle());

	boxViewLoc = glGetUniformLocation(boxShader->getProgramHandle(), "view");
	boxProjectionLoc = glGetUniformLocation(boxShader->getProgramHandle(), "projection");
	boxModelLoc = glGetUniformLocation(boxShader->getProgramHandle(), "model");
	boxColorLoc = glGetUniformLocation(boxShader->getProgramHandle(), "lineColor");

	glUseProgram(0);
}
