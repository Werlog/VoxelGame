#include "player.h"
#include <SDL2/SDL.h>
#include "world.h"
#include "imgui.h"

Player::Player(Camera* camera, World* world, ResourceManager& resourceManager)
	: blockOutline(&(resourceManager.getShader("shaders\\outline"))),
		collider(glm::vec3(0), glm::vec3(0))
{
	this->camera = camera;
	this->relPosition = glm::vec3(CHUNK_SIZE_X / 2.0f + 0.5f, 40, CHUNK_SIZE_Z / 2.0f + 0.5f);
	this->world = world;
	this->velocity = glm::vec3(0);
	this->acceleration = glm::vec3(0);

	this->chunkPosition = ChunkCoord{ 0, 0, 0 };
	playerMoved();

	isGrounded = false;

	enableCollision = true;
	enableFlight = false;

	selectedBlock = BlockType::STONE;
}

void Player::update(InputHandler& inputHandler, float deltaTime)
{
	camera->update(deltaTime);

	glm::vec3 friction = getFriction();
	velocity += acceleration * friction * deltaTime;


	checkGround();
	movement(inputHandler, deltaTime);
	resolveCollisions(deltaTime);
	relPosition += velocity * deltaTime;
	playerMoved();
	glm::vec3 worldPos = getWorldPosition();
	camera->position = relPosition + glm::vec3(0.0f, cameraHeight, 0.0f);

	ImGui::Begin("Test GUI");
	ImGui::Text("Position: %f, %f, %f", worldPos.x, worldPos.y, worldPos.z);
	ImGui::Text("Rel Position: %f, %f, %f", relPosition.x, relPosition.y, relPosition.z);
	ImGui::Text("Chunk Coord: %d, %d, %d", chunkPosition.x, chunkPosition.y, chunkPosition.z);
	ImGui::End();

	if (inputHandler.getMouseButtonDown(SDL_BUTTON_LEFT))
	{
		blockBreakLogic();
	}
	if (inputHandler.getMouseButtonDown(SDL_BUTTON_RIGHT))
	{
		blockPlaceLogic();
	}
	blockSwitchLogic(inputHandler);
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

BlockType Player::getSelectedBlock() const
{
	return selectedBlock;
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

void Player::movement(InputHandler& inputHandler, float deltaTime)
{
	glm::vec3 input = getInputDirection(inputHandler);

	float yaw = camera->yaw;

	glm::vec3 forward = glm::vec3(cos(glm::radians(yaw)), 0.0f, sin(glm::radians(yaw)));
	glm::vec3 right = glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f));

	acceleration = input.y * forward + input.x * right;
	if (acceleration != glm::vec3(0))
	{
		acceleration = glm::normalize(acceleration);
		acceleration *= enableFlight ? flightSpeed : playerSpeed;
	}

	glm::vec3 friction = getFriction();

	if (!enableFlight)
		velocity.y -= gravity * deltaTime;

	velocity -= minAbsVector(velocity, velocity * friction * deltaTime);

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
					if (world->getBlockAt(worldX, worldY, worldZ) == BlockType::AIR)
					{
						continue;
					}

					AABB blockAABB = AABB(glm::vec3(x, y, z), glm::vec3(x + 1, y + 1, z + 1));
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
			float epsilon = std::max(0.01f, glm::length(velocity) * 0.01f);

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
	glm::vec3 worldPos = getWorldPosition();
	for (float x = -playerWidth * 0.4f; x <= playerWidth * 0.4f; x += playerWidth * 0.4f)
	{
		for (float z = -playerWidth * 0.4f; z <= playerWidth * 0.4f; z += playerWidth * 0.4f)
		{
			isGrounded = world->getBlockAt((int)floor(worldPos.x + x), (int)floor(worldPos.y - 0.1f), (int)floor(worldPos.z + z)) != BlockType::AIR;
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
		return glm::vec3(18.0f);
	}

	if (velocity.y > 0.0f)
	{
		return glm::vec3(1.8f, 0.0f, 1.8f);
	}

	return glm::vec3(1.8f, 0.0f, 1.8f);
}

glm::vec3 Player::minAbsVector(const glm::vec3& vec1, const glm::vec3& vec2)
{
	glm::vec3 absVec1 = glm::vec3(abs(vec1.x), abs(vec1.y), abs(vec1.z));
	glm::vec3 absVec2 = glm::vec3(abs(vec2.x), abs(vec2.y), abs(vec2.z));

	if (glm::min(absVec1, absVec2) == absVec1)
	{
		return glm::vec3(vec1);
	}
	return glm::vec3(vec2);
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
	{
		int relX = placeX - chunkPosition.x * CHUNK_SIZE_X;
		int relY = placeY - chunkPosition.y * CHUNK_SIZE_Y;
		int relZ = placeZ - chunkPosition.z * CHUNK_SIZE_Z;
		AABB blockAABB = AABB(glm::vec3(relX, relY, relZ), glm::vec3(relX + 1, relY + 1, relZ + 1));
		if (enableCollision && blockAABB.isOverlapping(collider))
			return;

		world->modifyBlockAt(placeX, placeY, placeZ, selectedBlock);
	}
}

void Player::blockSwitchLogic(InputHandler& inputHandler)
{
	unsigned char block = ((unsigned char)selectedBlock) + inputHandler.getMouseScroll();

	if (block >= world->getBlockData().getData().size())
	{
		block = 1;
	}
	else if (block < 1)
	{
		block = world->getBlockData().getData().size() - 1;
	}

	selectedBlock = (BlockType)block;
}

std::unique_ptr<glm::vec3> Player::getLookingAtPosition()
{
	const glm::vec3& direction = camera->front;
	const glm::vec3& startPosition = camera->position + glm::vec3(chunkPosition.x * CHUNK_SIZE_X, chunkPosition.y * CHUNK_SIZE_Y, chunkPosition.z * CHUNK_SIZE_Z);
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
