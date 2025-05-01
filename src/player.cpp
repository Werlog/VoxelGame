#include "player.h"
#include <SDL2/SDL.h>
#include "world.h"
#include "imgui.h"

Player::Player(Camera* camera, World* world, ResourceManager& resourceManager)
	: blockOutline(&(resourceManager.getShader("shaders\\outline")))
{
	this->camera = camera;
	this->position = glm::vec3(0);
	this->world = world;
}

void Player::update(InputHandler& inputHandler, float deltaTime)
{
	camera->update(deltaTime);

	movement(inputHandler, deltaTime);

	ChunkCoord currentCoord = ChunkCoord::toChunkCoord(camera->position);

	ImGui::Begin("Debug UI");
	ImGui::Text("Position: %f, %f, %f ", camera->position.x, camera->position.y, camera->position.z);
	ImGui::Text("Chunk Coord: %d, %d, %d", currentCoord.x, currentCoord.y, currentCoord.z);
	ImGui::End();

	if (inputHandler.getMouseButtonDown(SDL_BUTTON_LEFT))
	{
		blockBreakLogic();
	}

	if (inputHandler.getMouseButtonDown(SDL_BUTTON_RIGHT))
	{
		blockPlaceLogic();
	}

	camera->position = position + glm::vec3(0.0f, playerHeight, 0.0f);
}

void Player::render()
{
	std::unique_ptr<glm::vec3> lookPos = getLookingAtPosition();

	if (lookPos == nullptr)
		return;

	glm::ivec3 blockPos = glm::ivec3((int)floor(lookPos->x), (int)floor(lookPos->y), (int)floor(lookPos->z));

	blockOutline.setPosition(blockPos);
	blockOutline.render(*camera);
}

const glm::vec3& Player::getPosition() const
{
	return position;
}

void Player::movement(InputHandler& inputHandler, float deltaTime)
{
	glm::vec3 input = getInputDirection(inputHandler);
	if (input.x == 0 && input.y == 0) return;

	glm::vec3 movementDirection = glm::normalize(camera->front * input.y + camera->right * input.x);
	if (inputHandler.getKey(SDLK_LSHIFT))
	{
		movementDirection *= 5.0f;
	}

	position += movementDirection * 8.0f * deltaTime;
}

glm::vec3 Player::getInputDirection(InputHandler& inputHandler)
{
	glm::vec3 input = glm::vec3(0);

	if (inputHandler.getKey(SDLK_w))
	{
		input.y += 1.0f;
	}
	if (inputHandler.getKey(SDLK_s))
	{
		input.y -= 1.0f;
	}
	if (inputHandler.getKey(SDLK_a))
	{
		input.x -= 1.0f;
	}
	if (inputHandler.getKey(SDLK_d))
	{
		input.x += 1.0f;
	}

	return input;
}

void Player::blockBreakLogic()
{
	std::unique_ptr<glm::vec3> blockPos = getLookingAtPosition();
	if (blockPos == nullptr)
		return;

	int breakX = (int)floor(blockPos->x);
	int breakY = (int)floor(blockPos->y);
	int breakZ = (int)floor(blockPos->z);

	world->modifyBlockAt(breakX, breakY, breakZ, BlockType::AIR);
}

void Player::blockPlaceLogic()
{
	std::unique_ptr<glm::vec3> blockPos = getLookingAtPosition();
	if (blockPos == nullptr)
		return;

	glm::vec3 centerPos = glm::vec3(floor(blockPos->x) + 0.5f, floor(blockPos->y) + 0.5f, floor(blockPos->z) + 0.5f);
	glm::vec3 direction = glm::normalize((*blockPos) - centerPos);

	glm::vec3 compass[] =
	{
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
	};

	glm::vec3& highestDir = compass[0];
	float highestDot = -1.1f;
	for (const glm::vec3& dir : compass)
	{
		float dot = glm::dot(dir, direction);

		if (dot > highestDot)
		{
			highestDot = dot;
			highestDir = dir;
		}
	}
	
	int placeX = (int)floor(centerPos.x + highestDir.x);
	int placeY = (int)floor(centerPos.y + highestDir.y);
	int placeZ = (int)floor(centerPos.z + highestDir.z);

	if (world->getBlockAt(placeX, placeY, placeZ) == BlockType::AIR)
		world->modifyBlockAt(placeX, placeY, placeZ, BlockType::STONE);
}

std::unique_ptr<glm::vec3> Player::getLookingAtPosition()
{
	const glm::vec3& direction = camera->front;
	const glm::vec3& startPosition = camera->position;
	for (float dist = 0.0f; dist < 5.0f; dist += 0.03f)
	{
		glm::vec3 checkPos = startPosition + direction * dist;

		int checkX = (int)floor(checkPos.x);
		int checkY = (int)floor(checkPos.y);
		int checkZ = (int)floor(checkPos.z);

		if (world->getBlockAt(checkX, checkY, checkZ) != BlockType::AIR)
		{
			return std::make_unique<glm::vec3>(checkPos);
		}
	}

	return nullptr;
}
