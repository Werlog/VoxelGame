#pragma once

#include <glm/glm.hpp>
#include "camera.h"
#include "inputhandler.h"
#include <memory>
#include "blockoutline.h"
#include "resourcemanager.h"

constexpr float playerHeight = 1.8f;

class World;

class Player
{
public:

	Player(Camera* camera, World* world, ResourceManager& resourceManager);

	void update(InputHandler& inputHandler, float deltaTime);
	void render();

	const glm::vec3& getPosition() const;
private:
	Camera* camera;
	glm::vec3 position;
	World* world;

	BlockOutline blockOutline;

	void movement(InputHandler& inputHandler, float deltaTime);
	glm::vec3 getInputDirection(InputHandler& inputHandler);

	void blockBreakLogic();
	void blockPlaceLogic();

	std::unique_ptr<glm::vec3> getLookingAtPosition();
};
