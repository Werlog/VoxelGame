#pragma once

#include <glm/glm.hpp>

struct CollisionResult
{
	float entryTime;
	glm::vec3 normal;
};

struct RayHit
{
	float entryTime;
	glm::vec3 normal;
	glm::vec3 hitPoint;
};
