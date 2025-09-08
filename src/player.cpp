#include "player.h"
#include <SDL2/SDL.h>
#include "world.h"
#include "imgui.h"

Player::Player(Camera* camera, World* world, Hotbar* hotbar, ResourceManager& resourceManager)
	: blockOutline(&(resourceManager.getShader("shaders\\outline"))),
		collider(glm::vec3(0), glm::vec3(0))
{
	this->camera = camera;
	this->relPosition = glm::vec3(0, 40, 0);
	this->world = world;
	this->velocity = glm::vec3(0);
	this->acceleration = glm::vec3(0);
	this->hotbar = hotbar;

	this->chunkPosition = ChunkCoord{ 0, 0, 0 };
	playerMoved();

	isGrounded = false;

	enableCollision = true;
	enableFlight = false;
}

void Player::update(InputHandler& inputHandler, float deltaTime)
{
	camera->update(deltaTime);

	movement(inputHandler, deltaTime);

	glm::vec3 friction = getFriction();

	velocity += acceleration * friction * deltaTime;

	// Damping
	glm::vec3 dampingFactor = glm::exp(-friction * deltaTime);
	dampingFactor = glm::clamp(dampingFactor, 0.0f, 1.0f);
	velocity = glm::mix(velocity, glm::vec3(0.0f), glm::vec3(1.0f) - dampingFactor);

	if (glm::length(velocity) < 0.05f)
		velocity = glm::vec3(0);

	if (!enableFlight && deltaTime < 1.0f) // Temporary fix for the player flying high when there is a big lag spike
		velocity.y -= gravity * deltaTime;

	acceleration = glm::vec3(0.0f);

	checkGround();
	resolveCollisions(deltaTime);

	relPosition += velocity * deltaTime;
	playerMoved();
	glm::vec3 worldPos = getWorldPosition();
	camera->setPosition(relPosition + glm::vec3(0.0f, cameraHeight, 0.0f));

	/*
	ImGui::SetNextWindowCollapsed(false);

	ImGui::Begin("Test GUI");
	ImGui::Text("Position: %f, %f, %f", worldPos.x, worldPos.y, worldPos.z);
	ImGui::Text("Rel Position: %f, %f, %f", relPosition.x, relPosition.y, relPosition.z);
	ImGui::Text("Chunk Coord: %d, %d, %d", chunkPosition.x, chunkPosition.y, chunkPosition.z);
	ImGui::End();
	*/
	if (inputHandler.getMouseButtonDown(SDL_BUTTON_LEFT))
	{
		blockBreakLogic();
	}
	if (inputHandler.getMouseButtonDown(SDL_BUTTON_RIGHT))
	{
		blockPlaceLogic();
	}
	if (inputHandler.getMouseButtonDown(SDL_BUTTON_MIDDLE))
	{
		blockPickLogic();
	}
}

void Player::render()
{
	std::unique_ptr<glm::vec3> lookPos = getLookingAtPosition();

	if (lookPos == nullptr)
		return;

	glm::ivec3 blockPos = glm::ivec3((int)floor(lookPos->x), (int)floor(lookPos->y), (int)floor(lookPos->z));
	blockOutline.setPosition(blockPos);
	blockOutline.render(*camera, chunkPosition);
}

void Player::setEnableCollision(bool enable)
{
	enableCollision = enable;
}

void Player::setEnableFlight(bool enable)
{
	enableFlight = enable;
}

void Player::setWorldPosition(glm::vec3 position)
{
	chunkPosition = ChunkCoord{ 0, 0, 0 };
	relPosition = position;
	playerMoved();
}

void Player::setRotation(float yaw, float pitch)
{
	camera->setYaw(yaw);
	camera->setPitch(pitch);
}

void Player::setHotbarItems(BlockType(&slots)[9])
{
	hotbar->setBlocks(slots);
}

const glm::vec3& Player::getRelPosition() const
{
	return relPosition;
}

const ChunkCoord& Player::getChunkPosition() const
{
	return chunkPosition;
}

glm::vec3 Player::getWorldPosition() const
{
	return glm::vec3(relPosition.x + chunkPosition.x * CHUNK_SIZE_X, relPosition.y + chunkPosition.y * CHUNK_SIZE_Y, relPosition.z + chunkPosition.z * CHUNK_SIZE_Z);
}

bool Player::getEnableCollision() const
{
	return enableCollision;
}

bool Player::getEnableFlight() const
{
	return enableFlight;
}

float Player::getYaw() const
{
	return camera->getYaw();
}

float Player::getPitch() const
{
	return camera->getPitch();
}

BlockType* Player::getHotbarItems()
{
	return hotbar->getBlocks();
}

void Player::movement(InputHandler& inputHandler, float deltaTime)
{
	glm::vec3 input = getInputDirection(inputHandler);

	float yaw = camera->getYaw();

	glm::vec3 forward = glm::vec3(cos(glm::radians(yaw)), 0.0f, sin(glm::radians(yaw)));
	glm::vec3 right = glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f));

	acceleration = input.y * forward + input.x * right;

	if (acceleration != glm::vec3(0))
	{
		acceleration = glm::normalize(acceleration);
		acceleration *= enableFlight ? flightSpeed : playerSpeed;

		if (enableFlight && inputHandler.getKey(SDLK_LCTRL))
		{
			acceleration *= 5.0f;
		}
	}

	//velocity -= minAbsVector(velocity, velocity * friction * deltaTime);

	if (enableFlight && inputHandler.getKey(SDLK_SPACE))
	{
		acceleration.y += flightSpeed * 0.75f;
	}
	if (enableFlight && inputHandler.getKey(SDLK_LSHIFT))
	{
		acceleration.y -= flightSpeed * 0.75f;
	}

	if (inputHandler.getKey(SDLK_SPACE) && isGrounded && !enableFlight)
	{
		velocity.y = sqrt(2 * gravity * jumpHeight);
	}
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

void Player::playerMoved()
{
	updateRelPosition();
	updateCollider();
}

void Player::updateRelPosition()
{
	ChunkCoord coord = ChunkCoord::toChunkCoord(relPosition);

	if (coord.x != 0 || coord.y != 0 || coord.z != 0)
	{
		chunkPosition += coord;
		relPosition -= glm::vec3(coord.x * CHUNK_SIZE_X, coord.y * CHUNK_SIZE_Y, coord.z * CHUNK_SIZE_Z);
	}
}

void Player::updateCollider()
{
	glm::vec3 min = glm::vec3(relPosition.x - playerWidth * 0.5f, relPosition.y, relPosition.z - playerWidth * 0.5f);
	glm::vec3 max = glm::vec3(relPosition.x + playerWidth * 0.5f, relPosition.y + playerHeight, relPosition.z + playerWidth * 0.5f);

	collider.setMin(min);
	collider.setMax(max);
}

void Player::resolveCollisions(float deltaTime)
{
	if (!enableCollision) return;

	const glm::vec3& min = collider.getMin();
	const glm::vec3& max = collider.getMax();

	glm::vec3 velocity = this->velocity * deltaTime;

	BlockData& blockData = world->getBlockData();

	for (int _ = 0; _ < 3; _++)
	{
		// Broad phase box
		float startX = velocity.x > 0 ? min.x : min.x + velocity.x;
		float startY = velocity.y > 0 ? min.y : min.y + velocity.y;
		float startZ = velocity.z > 0 ? min.z : min.z + velocity.z;

		float endX = velocity.x > 0 ? velocity.x + max.x : max.x - velocity.x;
		float endY = velocity.y > 0 ? velocity.y + max.y : max.y - velocity.y;
		float endZ = velocity.z > 0 ? velocity.z + max.z : max.z - velocity.z;

		CollisionResult nearest = CollisionResult
		{
			1.0f,
			glm::vec3(0.0f),
		};

		for (int x = (int)floor(startX); x <= (int)floor(endX); x++)
		{
			for (int y = (int)floor(startY); y <= (int)floor(endY); y++)
			{
				for (int z = (int)floor(startZ); z <= (int)floor(endZ); z++)
				{
					int worldX = x + chunkPosition.x * CHUNK_SIZE_X;
					int worldY = y + chunkPosition.y * CHUNK_SIZE_Y;
					int worldZ = z + chunkPosition.z * CHUNK_SIZE_Z;

					BlockType blockType = world->getBlockAt(worldX, worldY, worldZ);
					if (Block::isSimilar(blockType, BlockType::AIR))
					{
						continue;
					}
					const std::shared_ptr<Block>& block = blockData.getBlock(blockType);

					AABB blockAABB = block->getCollider(glm::ivec3(x, y, z), blockType);
					CollisionResult collision = collider.collide(blockAABB, velocity);

					if (collision.entryTime < nearest.entryTime)
					{
						nearest = collision;
					}
				}
			}
		}

		if (nearest.entryTime < 1.0f)
		{
			float epsilon = std::max(0.05f, glm::length(velocity) * 0.03f);

			//this->velocity = glm::vec3(0.0f);

			
			glm::vec3 move = velocity * (nearest.entryTime - epsilon);
			relPosition += move;
			playerMoved();

			float dot = glm::dot(velocity, nearest.normal);
			velocity -= nearest.normal * dot;
			this->velocity = velocity / deltaTime;
			
		}
	}
}

void Player::checkGround()
{
	isGrounded = false;

	BlockData& blockData = world->getBlockData();

	glm::vec3 worldPos = getWorldPosition();
	for (float x = -playerWidth * 0.4f; x <= playerWidth * 0.4f; x += playerWidth * 0.4f)
	{
		for (float z = -playerWidth * 0.4f; z <= playerWidth * 0.4f; z += playerWidth * 0.4f)
		{
			int blockPosX = (int)floor(worldPos.x + x);
			int blockPosY = (int)floor(worldPos.y - 0.1f);
			int blockPosZ = (int)floor(worldPos.z + z);

			BlockType blockType = world->getBlockAt(blockPosX, blockPosY, blockPosZ);

			if (blockType == BlockType::AIR) continue;

			const std::shared_ptr<Block>& block = blockData.getBlock(blockType);
			AABB aabb = block->getCollider(glm::ivec3(blockPosX, blockPosY, blockPosZ), blockType);

			isGrounded = aabb.isInside(glm::vec3(worldPos.x + x, worldPos.y - 0.1f, worldPos.z + z));

			if (isGrounded) return;
		}
	}
}

glm::vec3 Player::getFriction()
{
	if (enableFlight)
	{
		return glm::vec3(6.0f);
	}

	if (isGrounded)
	{
		return glm::vec3(13.0f, 0.0f, 13.0f);
	}

	return glm::vec3(1.0f, 0.0f, 1.0f);
}

void Player::blockBreakLogic()
{
	std::unique_ptr<glm::vec3> blockPos = getLookingAtPosition();
	if (blockPos == nullptr)
		return;

	int breakX = (int)floor(blockPos->x);
	int breakY = (int)floor(blockPos->y);
	int breakZ = (int)floor(blockPos->z);

	world->modifyBlockAt(breakX, breakY, breakZ, BlockType::AIR, true);
}

void Player::blockPlaceLogic()
{
	std::unique_ptr<glm::vec3> blockPos = getLookingAtPosition();
	if (blockPos == nullptr)
		return;

	BlockData& blockData = world->getBlockData();
	const std::shared_ptr<Block>& blockToPlace = blockData.getBlock(hotbar->getSelectedBlock());

	BlockPlaceResult result = blockToPlace->placeBlock(std::move(blockPos), *world);

	if (!result.success)
		return;

	// Check if block collider overlaps with player collider
	glm::ivec3 relativeBlockPos = glm::ivec3(result.placedPosition.x - chunkPosition.x * CHUNK_SIZE_X, result.placedPosition.y - chunkPosition.y * CHUNK_SIZE_Y, result.placedPosition.z - chunkPosition.z * CHUNK_SIZE_Z);
	if (blockToPlace->getCollider(relativeBlockPos, result.placedBlock).isOverlapping(collider))
	{
		return;
	}

	world->modifyBlockAt(result.placedPosition.x, result.placedPosition.y, result.placedPosition.z, result.placedBlock);

	blockToPlace->onUpdate(result.placedPosition, result.placedBlock, *world);
}

void Player::blockPickLogic()
{
	std::unique_ptr<glm::vec3> blockPos = getLookingAtPosition();
	if (blockPos == nullptr)
		return;

	int pickX = (int)floor(blockPos->x);
	int pickY = (int)floor(blockPos->y);
	int pickZ = (int)floor(blockPos->z);

	BlockType type = world->getBlockAt(pickX, pickY, pickZ);
	if (type != BlockType::AIR)
		hotbar->handleMiddleClick(type);
}

std::unique_ptr<glm::vec3> Player::getLookingAtPosition()
{
	const glm::vec3& direction = camera->front;
	const glm::vec3& startPosition = camera->getPosition() + glm::vec3(chunkPosition.x * CHUNK_SIZE_X, chunkPosition.y * CHUNK_SIZE_Y, chunkPosition.z * CHUNK_SIZE_Z);
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
