#pragma once

#include <glm/glm.hpp>
#include "shader.h"
#include "camera.h"
#include "chunkcoord.h"

class BlockOutline
{
public:

	BlockOutline(Shader* shader);

	void render(Camera& camera, const ChunkCoord& playerCoord);

	void setPosition(const glm::ivec3 newPosition);
private:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	unsigned int viewLoc;
	unsigned int modelLoc;
	unsigned int projectionLoc;

	Shader* outlineShader;

	glm::ivec3 position;

	void createMesh();
	void setupShader();
};
