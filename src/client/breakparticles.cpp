#include "breakparticles.h"
#include <glad/glad.h>
#include <random>
#include <glm/gtc/type_ptr.hpp>
#include "world.h"

BreakParticles::BreakParticles(BlockType block, BlockData& blockData, const glm::vec3& position, ResourceManager& resourceManager, TextureSheet& terrainSheet)
{
	this->particleShader = &resourceManager.getShader("shaders/blockbreakparticle");
	this->terrainSheet = &terrainSheet;
	
	this->VAO = 0;
	this->VBO = 0;
	this->EBO = 0;
	this->instanceVBO = 0;
	this->blockTexture = blockData.getBlockProperties(block).frontFaceTexId;

	this->position = position;
	this->playerCoord = ChunkCoord{ 0, 0, 0 };

	this->viewLoc = 0;
	this->projectionLoc = 0;
	this->blockTypeLoc = 0;
	this->camRightLoc = 0;
	this->camUpLoc = 0;

	init();
}

BreakParticles::~BreakParticles()
{
	if (this->VAO != 0) glDeleteVertexArrays(1, &VAO);

	if (this->VBO != 0) glDeleteBuffers(1, &VBO);
	if (this->EBO != 0) glDeleteBuffers(1, &EBO);
	if (this->instanceVBO != 0) glDeleteBuffers(1, &instanceVBO);
}

BreakParticles::BreakParticles(BreakParticles&& other) noexcept
{
	this->position = other.position;
	this->playerCoord = other.playerCoord;
	this->blockTexture = other.blockTexture;
	this->VAO = other.VAO;
	this->VBO = other.VBO;
	this->EBO = other.EBO;
	this->instanceVBO = other.instanceVBO;
	this->viewLoc = other.viewLoc;
	this->projectionLoc = other.projectionLoc;
	this->blockTypeLoc = other.blockTypeLoc;
	this->camRightLoc = other.camRightLoc;
	this->camUpLoc = other.camUpLoc;
	this->particles = std::move(other.particles);
	this->particleShader = other.particleShader;
	this->terrainSheet = other.terrainSheet;

	other.VAO = 0;
	other.VBO = 0;
	other.EBO = 0;
	other.instanceVBO = 0;
}

BreakParticles& BreakParticles::operator=(BreakParticles&& other) noexcept
{
	if (this != &other)
	{
		if (this->VAO != 0) glDeleteVertexArrays(1, &VAO);
		if (this->VBO != 0) glDeleteBuffers(1, &VBO);
		if (this->EBO != 0) glDeleteBuffers(1, &EBO);
		if (this->instanceVBO != 0) glDeleteBuffers(1, &instanceVBO);

		this->position = other.position;
		this->playerCoord = other.playerCoord;
		this->blockTexture = other.blockTexture;
		this->VAO = other.VAO;
		this->VBO = other.VBO;
		this->EBO = other.EBO;
		this->instanceVBO = other.instanceVBO;
		this->viewLoc = other.viewLoc;
		this->projectionLoc = other.projectionLoc;
		this->blockTypeLoc = other.blockTypeLoc;
		this->camRightLoc = other.camRightLoc;
		this->camUpLoc = other.camUpLoc;
		this->particles = std::move(other.particles);
		this->particleShader = other.particleShader;
		this->terrainSheet = other.terrainSheet;

		other.VAO = 0;
		other.VBO = 0;
		other.EBO = 0;
		other.instanceVBO = 0;
	}

	return *this;
}

void BreakParticles::update(float deltaTime, const ChunkCoord& playerCoord, World& world)
{
	this->playerCoord = playerCoord;
	
	ChunkCoord coord = ChunkCoord::toChunkCoord(position);
	glm::vec3 chunkWorldPos = glm::vec3(coord.x * CHUNK_SIZE_X, coord.y * CHUNK_SIZE_Y, coord.z * CHUNK_SIZE_Z);
	
	bool removed = false;
	for (auto it = particles.begin(); it != particles.end();)
	{
		BreakParticle& particle = *it;

		particle.lifetime -= deltaTime;
		if (particle.lifetime <= 0.0f)
		{
			it = particles.erase(it);
			removed = true;
			continue;
		}

		particle.velocity.y -= 29.0f * deltaTime;

		int blockX = floor(chunkWorldPos.x + particle.transform.x);
		int blockY = floor(chunkWorldPos.y + particle.transform.y - particle.transform.w);
		int blockZ = floor(chunkWorldPos.z + particle.transform.z);

		if (world.getBlockAt(blockX, blockY, blockZ) != BlockType::AIR)
		{
			particle.velocity = glm::vec3(0.0f);
		}

		particle.transform = glm::vec4(particle.transform.x + particle.velocity.x * deltaTime, particle.transform.y + particle.velocity.y * deltaTime, particle.transform.z + particle.velocity.z * deltaTime, particle.transform.w);

		it++;
	}

	if (removed)
		updateInstanceBuffer();
	else
		updateInstancePositions();
}

void BreakParticles::render(Camera& camera)
{
	glUseProgram(particleShader->getProgramHandle());
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D_ARRAY, terrainSheet->getTextureHandle());

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));
	glUniform3fv(camUpLoc, 1, glm::value_ptr(camera.up));
	glUniform3fv(camRightLoc, 1, glm::value_ptr(camera.right));
	glUniform1i(blockTypeLoc, (int)blockTexture);

	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, particles.size());

	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

bool BreakParticles::isFinished() const
{
	return particles.size() <= 0;
}

void BreakParticles::init()
{
	createParticles();
	initUniforms();

	constexpr float quadVertices[] = 
	{	// x, y, z, u, v
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
	};

	constexpr int quadIndices[] =
	{
		0, 1, 2,
		1, 3, 2,
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &instanceVBO);
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(4 * sizeof(float)));

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	updateInstanceBuffer();

	glBindVertexArray(0);
}

void BreakParticles::initUniforms()
{
	glUseProgram(particleShader->getProgramHandle());

	viewLoc = glGetUniformLocation(particleShader->getProgramHandle(), "view");
	projectionLoc = glGetUniformLocation(particleShader->getProgramHandle(), "projection");
	blockTypeLoc = glGetUniformLocation(particleShader->getProgramHandle(), "blockType");
	camRightLoc = glGetUniformLocation(particleShader->getProgramHandle(), "camRight");
	camUpLoc = glGetUniformLocation(particleShader->getProgramHandle(), "camUp");

	glUseProgram(0);
}

void BreakParticles::updateInstanceBuffer()
{
	std::vector<float> bufferData;
	bufferData.reserve(particles.size() * 6);

	for (size_t i = 0; i < particles.size(); i++)
	{
		BreakParticle& particle = particles[i];

		const float* transformPtr = glm::value_ptr(particle.transform);
		const float* uvOffsetPtr = glm::value_ptr(particle.uvOffset);

		bufferData.insert(bufferData.end(), transformPtr, transformPtr + 4);
		bufferData.insert(bufferData.end(), uvOffsetPtr, uvOffsetPtr + 2);
	}

	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

	glBufferData(GL_ARRAY_BUFFER, bufferData.size() * sizeof(float), bufferData.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BreakParticles::updateInstancePositions()
{
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

	ChunkCoord chunkDiff = ChunkCoord::toChunkCoord(position) - playerCoord;
	glm::vec3 posDiff = glm::vec3(chunkDiff.x * CHUNK_SIZE_X, chunkDiff.y * CHUNK_SIZE_Y, chunkDiff.z * CHUNK_SIZE_Z);

	for (size_t i = 0; i < particles.size(); i++)
	{
		BreakParticle& particle = particles[i];
		size_t offset = i * 6 * sizeof(float);

		glm::vec4 renderPos = glm::vec4(posDiff, 0.0f) + particle.transform;

		glBufferSubData(GL_ARRAY_BUFFER, offset, 4 * sizeof(float), glm::value_ptr(renderPos));
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BreakParticles::createParticles()
{
	std::random_device r;
	std::mt19937 rng(r());

	std::uniform_int_distribution<int> amountDist(60, 80);
	std::uniform_real_distribution<float> positionDist(0.0f, 1.0f);
	std::uniform_real_distribution<float> scaleDist(0.05f, 0.12f);
	std::uniform_real_distribution<float> lifetimeDist(1.5f, 2.5f);
	std::uniform_real_distribution<float> yVelDist(3.0f, 7.0f);
	std::uniform_real_distribution<float> horizontalVelDist(1.0f, 7.0f);

	int particleAmount = amountDist(rng);

	particles.resize(particleAmount);

	ChunkCoord coord = ChunkCoord::toChunkCoord(position);
	glm::vec3 relPos = glm::vec3(position.x - coord.x * CHUNK_SIZE_X, position.y - coord.y * CHUNK_SIZE_Y, position.z - coord.z * CHUNK_SIZE_Z);

	for (size_t i = 0; i < particles.size(); i++)
	{
		BreakParticle& particle = particles[i];

		particle.transform = glm::vec4(relPos.x + positionDist(rng), relPos.y + positionDist(rng), relPos.z + positionDist(rng), scaleDist(rng));

		std::uniform_real_distribution<float> uvDist(0.0f, 1.0f - particle.transform.w);
		particle.uvOffset = glm::vec2(uvDist(rng), uvDist(rng));
		particle.lifetime = lifetimeDist(rng);

		glm::vec3 direction = glm::vec3(particle.transform) + glm::vec3(-0.5f, 0.5f, -0.5f) - relPos;
		direction = glm::normalize(direction) * horizontalVelDist(rng);
		direction.y = yVelDist(rng);
		particle.velocity = direction;
	}
}
