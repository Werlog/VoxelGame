#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include "resourcemanager.h"
#include "shader.h"
#include "camera.h"

class DebugRenderer
{
public:

	DebugRenderer();

	void init(ResourceManager& resourceManager, Camera* camera);

	void renderBox(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color = glm::vec3(1.0f, 0.0f, 0.0f));
private:
	Shader* boxShader;
	Camera* camera;

	unsigned int boxVAO;
	unsigned int boxVBO;
	unsigned int boxEBO;

	unsigned int boxViewLoc;
	unsigned int boxModelLoc;
	unsigned int boxProjectionLoc;
	unsigned int boxColorLoc;

	void setupBox(ResourceManager& resourceManager);
};
