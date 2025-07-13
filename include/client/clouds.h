#pragma once

#include "shader.h"
#include "resourcemanager.h"
#include "camera.h"

constexpr float cloudPlaneSize = 700.0f;

class Clouds
{
public:

	Clouds(ResourceManager& resourceManager);

	void update(float deltaTime);
	void render(Camera* camera, const glm::vec3& playerPosition);
private:
	Shader* cloudShader;
	Texture* cloudTexture;

	float timeOffset;

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	unsigned int projectionLoc;
	unsigned int viewLoc;
	unsigned int playerPosLoc;
	unsigned int planeSizeLoc;
	unsigned int timeLoc;
};
