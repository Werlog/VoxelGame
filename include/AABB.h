#pragma once

#include <glm/glm.hpp>

class AABB
{
public:

	AABB(const glm::vec3& min, const glm::vec3& max);

	void move(const glm::vec3& amount);
	void setMin(const glm::vec3& newMin);
	void setMax(const glm::vec3& newMax);

	const glm::vec3& getMin() const;
	const glm::vec3& getMax() const;

	void collide(const AABB& other, const glm::vec3& velocity);
private:
	glm::vec3 min;
	glm::vec3 max;
};
