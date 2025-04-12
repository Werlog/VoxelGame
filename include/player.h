#pragma once

#include <glm/glm.hpp>
#include "camera.h"
#include "inputhandler.h"

constexpr float playerHeight = 1.8f;

class Player
{
public:

	Player(Camera* camera);

	void update(InputHandler& inputHandler, float deltaTime);
private:
	Camera* camera;
	glm::vec3 position;

	void movement(InputHandler& inputHandler, float deltaTime);
	glm::vec3 getInputDirection(InputHandler& inputHandler);
};
