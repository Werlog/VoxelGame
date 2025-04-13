#include "player.h"
#include <SDL2/SDL.h>

Player::Player(Camera* camera)
{
	this->camera = camera;
	this->position = glm::vec3(0);
}

void Player::update(InputHandler& inputHandler, float deltaTime)
{
	camera->update(deltaTime);

	movement(inputHandler, deltaTime);

	camera->position = position + glm::vec3(0.0f, playerHeight, 0.0f);
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
		movementDirection *= 2.0f;
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
