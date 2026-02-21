#pragma once

#include <glm/glm.hpp>
#include "camera.h"
#include "inputhandler.h"
#include <memory>
#include "blockoutline.h"
#include "resourcemanager.h"
#include "blockdata.h"
#include "AABB.h"
#include "debugRenderer.h"
#include "chunkcoord.h"
#include "UI/elements/HUD/hotbar.h"

class World;

class Player
{
public:
	static constexpr float playerHeight = 1.9f;
	static constexpr float cameraHeight = 1.8f;
	static constexpr float playerWidth = 0.6f;

	// Movement
	static constexpr float gravity = 32.0f;
	static constexpr float jumpHeight = 1.28f;
	static constexpr float playerSpeed = 7.5f;
	static constexpr float flightSpeed = 15.0f;

	Player(Camera* camera, World* world, Hotbar* hotbar, ResourceManager& resourceManager);

	void update(InputHandler& inputHandler, float deltaTime);
	void render();

	void setEnableCollision(bool enable);
	void setEnableFlight(bool enable);

	void setWorldPosition(glm::vec3 position);
	void setRotation(float yaw, float pitch);
	void setHotbarItems(BlockType (&slots)[9]);

	const glm::vec3& getRelPosition() const;
	const ChunkCoord& getChunkPosition() const;
	glm::vec3 getWorldPosition() const;
	bool getEnableCollision() const;
	bool getEnableFlight() const;
	bool isPlayerGrounded() const;
	glm::vec3 getVelocity() const;
	float getYaw() const;
	float getPitch() const;

	BlockType* getHotbarItems();
private:
	Camera* camera;
	glm::vec3 relPosition;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	bool isGrounded;
	bool enableCollision;
	bool enableFlight;
	World* world;
	Hotbar* hotbar;

	ChunkCoord chunkPosition;

	// Collider relative to the current chunk's position
	AABB collider;

	BlockOutline blockOutline;

	void movement(InputHandler& inputHandler, float deltaTime);
	glm::vec3 getInputDirection(InputHandler& inputHandler);
	void playerMoved();

	void updateRelPosition();
	void updateCollider();
	void resolveCollisions(float deltaTime);
	void checkGround();
	glm::vec3 getFriction();

	void blockBreakLogic();
	void blockPlaceLogic();
	void blockPickLogic();

	std::unique_ptr<glm::vec3> getLookingAtPosition();
};
