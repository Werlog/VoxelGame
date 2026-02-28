#include "collision/ray.h"
#include "collision/AABB.h"
#include <algorithm>

Ray::Ray(glm::vec3 origin, glm::vec3 direction)
{
	this->origin = origin;
	this->direction = direction;
}

RayHit Ray::intersect(const AABB& aabb)
{
	glm::vec3 boxMin = aabb.getMin();
	glm::vec3 boxMax = aabb.getMax();

	float invDirX = 1.0f / direction.x;
	float entryX = (boxMin.x - origin.x) * invDirX;
	float exitX = (boxMax.x - origin.x) * invDirX;

	if (entryX > exitX) std::swap(entryX, exitX);

	float invDirY = 1.0f / direction.y;
	float entryY = (boxMin.y - origin.y) * invDirY;
	float exitY = (boxMax.y - origin.y) * invDirY;

	if (entryY > exitY) std::swap(entryY, exitY);

	float invDirZ = 1.0f / direction.z;
	float entryZ = (boxMin.z - origin.z) * invDirZ;
	float exitZ = (boxMax.z - origin.z) * invDirZ;

	if (entryZ > exitZ) std::swap(entryZ, exitZ);

	float entryTime = std::max({ entryX, entryY, entryZ });
	float exitTime = std::min({ exitX, exitY, exitZ });

	if (entryTime > exitTime || entryTime > 1.0f || entryTime < 0.0f)
	{
		return RayHit
		{
			1.0f,
			glm::vec3(0.0f),
			glm::vec3(0.0f)
		};
	}

	glm::vec3 normal = glm::vec3(0);
	if (entryX > entryY && entryX > entryZ)
	{
		normal = direction.x > 0 ? glm::vec3(-1.0f, 0.0f, 0.0f) : glm::vec3(1.0f, 0.0f, 0.0f);
	}
	else if (entryY > entryX && entryY > entryZ)
	{
		normal = direction.y > 0 ? glm::vec3(0.0f, -1.0f, 0.0f) : glm::vec3(0.0f, 1.0f, 0.0f);
	}
	else
	{
		normal = direction.z > 0 ? glm::vec3(0.0f, 0.0f, -1.0f) : glm::vec3(0.0f, 0.0f, 1.0f);
	}

	return RayHit
	{
		entryTime,
		normal,
		origin + direction * entryTime,
	};
}
