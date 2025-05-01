#include "blockoutline.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

BlockOutline::BlockOutline(Shader* shader)
{
	this->outlineShader = shader;

	VAO = 0;
	VBO = 0;
	EBO = 0;

	viewLoc = 0;
	modelLoc = 0;
	projectionLoc = 0;

	createMesh();
	setupShader();
}

void BlockOutline::render(Camera& camera)
{
	glUseProgram(outlineShader->getProgramHandle());

	glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(position));

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(VAO);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
}

void BlockOutline::setPosition(const glm::ivec3 newPosition)
{
	this->position = newPosition;
}

void BlockOutline::createMesh()
{
	if (VAO != 0)
		return;

	float vertices[] =
	{
		-0.01f, -0.01f, -0.01f,
		1.01f, -0.01f, -0.01f,
		-0.01f, 1.01f, -0.01f,
		1.01f, 1.01f, -0.01f,

		-0.01f, -0.01f, 1.01f,
		1.01f, -0.01f, 1.01f,
		-0.01f, 1.01f, 1.01f,
		1.01f, 1.01f, 1.01f,
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

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void BlockOutline::setupShader()
{
	glUseProgram(outlineShader->getProgramHandle());

	viewLoc = glGetUniformLocation(outlineShader->getProgramHandle(), "view");
	modelLoc = glGetUniformLocation(outlineShader->getProgramHandle(), "model");
	projectionLoc = glGetUniformLocation(outlineShader->getProgramHandle(), "projection");
}
