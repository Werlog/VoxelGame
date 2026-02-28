#pragma once

#include <glm/glm.hpp>
#include "collisionResult.h"

class AABB;

class Ray
{
public:

	Ray(glm::vec3 origin, glm::vec3 direction);

	RayHit intersect(const AABB& aabb);
private:
	glm::vec3 origin;
	glm::vec3 direction;
};
