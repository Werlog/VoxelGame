#pragma once

#include <glm/glm.hpp>
#include "camera.h"
#include "inputhandler.h"
#include <memory>

constexpr float playerHeight = 1.8f;

class World;

class Player
{
public:

	Player(Camera* camera, World* world);

	void update(InputHandler& inputHandler, float deltaTime);

	const glm::vec3& getPosition() const;
private:
	Camera* camera;
	glm::vec3 position;
	World* world;

	void movement(InputHandler& inputHandler, float deltaTime);
	glm::vec3 getInputDirection(InputHandler& inputHandler);

	void blockBreakLogic();
	void blockPlaceLogic();

	std::unique_ptr<glm::vec3> getLookingAtPosition();
};
