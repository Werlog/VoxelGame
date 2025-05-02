#pragma once

#include <glm/glm.hpp>
#include "camera.h"
#include "inputhandler.h"
#include <memory>
#include "blockoutline.h"
#include "resourcemanager.h"
#include "blockdata.h"

constexpr float playerHeight = 1.8f;

class World;

class Player
{
public:

	Player(Camera* camera, World* world, ResourceManager& resourceManager);

	void update(InputHandler& inputHandler, float deltaTime);
	void render();

	BlockType getSelectedBlock() const;
	const glm::vec3& getPosition() const;
private:
	Camera* camera;
	glm::vec3 position;
	World* world;

	BlockType selectedBlock;

	BlockOutline blockOutline;

	void movement(InputHandler& inputHandler, float deltaTime);
	glm::vec3 getInputDirection(InputHandler& inputHandler);

	void blockBreakLogic();
	void blockPlaceLogic();
	void blockSwitchLogic(InputHandler& inputHandler);

	std::unique_ptr<glm::vec3> getLookingAtPosition();
};
