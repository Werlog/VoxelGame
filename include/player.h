#pragma once

#include <glm/glm.hpp>
#include "camera.h"
#include "inputhandler.h"
#include <memory>
#include "blockoutline.h"
#include "resourcemanager.h"
#include "blockdata.h"
#include "AABB.h"

constexpr float playerHeight = 1.9f;
constexpr float cameraHeight = 1.8f;
constexpr float playerWidth = 0.6f;

// Movement
constexpr float gravity = 32.0f;
constexpr float jumpHeight = 1.5f;
constexpr float playerSpeed = 9.1f;

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
	glm::vec3 velocity;
	glm::vec3 acceleration;
	bool isGrounded;
	World* world;

	AABB collider;

	BlockType selectedBlock;

	BlockOutline blockOutline;

	void movement(InputHandler& inputHandler, float deltaTime);
	glm::vec3 getInputDirection(InputHandler& inputHandler);
	void updateCollider();
	void resolveCollisions(float deltaTime);
	void checkGround();
	glm::vec3 getFriction();
	glm::vec3 minAbsVector(const glm::vec3& vec1, const glm::vec3& vec2);

	void blockBreakLogic();
	void blockPlaceLogic();
	void blockSwitchLogic(InputHandler& inputHandler);

	std::unique_ptr<glm::vec3> getLookingAtPosition();
};
