#pragma once

#include "blockdata.h"
#include "resourcemanager.h"
#include "chunkcoord.h"
#include "camera.h"
#include <glm/glm.hpp>
#include "texturesheet.h"

class World;

class BreakParticles
{
public:

	BreakParticles(BlockType block, BlockData& blockData, const glm::vec3& position, ResourceManager& resourceManager, TextureSheet& terrainSheet);
	~BreakParticles();

	BreakParticles(const BreakParticles&) = delete;
	BreakParticles& operator=(const BreakParticles&) = delete;

	BreakParticles(BreakParticles&& other) noexcept;
	BreakParticles& operator=(BreakParticles&& other) noexcept;

	void update(float deltaTime, const ChunkCoord& playerCoord, World& world);
	void render(Camera& camera);

	bool isFinished() const;
private:
	glm::vec3 position;
	int blockTexture;
	ChunkCoord playerCoord;

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	unsigned int instanceVBO;

	// Uniforms
	unsigned int viewLoc;
	unsigned int projectionLoc;
	unsigned int blockTypeLoc;
	unsigned int camRightLoc;
	unsigned int camUpLoc;

	struct BreakParticle
	{
		glm::vec4 transform; // x, y, z = position, w = scale
		glm::vec3 velocity;
		glm::vec2 uvOffset;

		float lifetime;
	};

	std::vector<BreakParticle> particles;

	Shader* particleShader;
	TextureSheet* terrainSheet;

	void init();
	void initUniforms();

	void updateInstanceBuffer();
	void updateInstancePositions();

	void createParticles();
};
