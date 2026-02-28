#pragma once

#include <glm/glm.hpp>
#include "collisionResult.h"

class AABB
{
public:
	static constexpr float epsilon = 0.001f;

	AABB(const glm::vec3& min, const glm::vec3& max);
	AABB();

	AABB& move(const glm::vec3& amount);
	AABB& setMin(const glm::vec3& newMin);
	AABB& setMax(const glm::vec3& newMax);

	const glm::vec3& getMin() const;
	const glm::vec3& getMax() const;

	bool isOverlapping(const AABB& other) const;
	bool isInside(glm::vec3 point) const;

	CollisionResult collide(const AABB& other, const glm::vec3& velocity);
private:
	glm::vec3 min;
	glm::vec3 max;
};
