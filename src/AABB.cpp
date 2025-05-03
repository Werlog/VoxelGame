#include "AABB.h"
#include <limits>
#include <algorithm>
#include <iostream>

AABB::AABB(const glm::vec3& min, const glm::vec3& max)
{
	this->min = min;
	this->max = max;
}

void AABB::move(const glm::vec3& amount)
{
	min += amount;
	max += amount;
}

void AABB::setMin(const glm::vec3& newMin)
{
	this->min = newMin;
}

void AABB::setMax(const glm::vec3& newMax)
{
	this->max = newMax;
}

const glm::vec3& AABB::getMin() const
{
	return min;
}

const glm::vec3& AABB::getMax() const
{
	return max;
}

void AABB::collide(const AABB& other, const glm::vec3& velocity)
{
	const glm::vec3& otherMin = other.getMin();
	const glm::vec3& otherMax = other.getMax();

	// X Axis
	float xEntry, xExit;
	if (velocity.x > 0.0f)
	{
		xEntry = otherMin.x - max.x;
		xExit = otherMax.x - min.x;
	}
	else
	{
		xEntry = otherMax.x - min.x;
		xExit = otherMin.x - max.x;
	}

	// Y Axis
	float yEntry, yExit;
	if (velocity.y > 0.0f)
	{
		yEntry = otherMin.y - max.y;
		yExit = otherMax.y - min.y;
	}
	else
	{
		yEntry = otherMax.y - min.y;
		yExit = otherMin.y - max.y;
	}

	// Z Axis
	float zEntry, zExit;
	if (velocity.z > 0.0f)
	{
		zEntry = otherMin.z - max.z;
		zExit = otherMax.z - min.z;
	}
	else
	{
		zEntry = otherMax.z - min.z;
		zExit = otherMin.z - max.z;
	}

	if (velocity.x == 0.0f)
	{
		xEntry = -std::numeric_limits<float>::infinity();
		xExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		xEntry /= velocity.x;
		xExit /= velocity.x;
	}

	if (velocity.y == 0.0f)
	{
		yEntry = -std::numeric_limits<float>::infinity();
		yExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		yEntry /= velocity.y;
		yExit /= velocity.y;
	}

	if (velocity.z == 0.0f)
	{
		zEntry = -std::numeric_limits<float>::infinity();
		zExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		zEntry /= velocity.z;
		zExit /= velocity.z;
	}

	float entryTime = std::max({ xEntry, yEntry, zEntry });
	float exitTime = std::min({ xExit, yExit, zExit });

	if (entryTime > exitTime || xEntry < 0.0f && yEntry < 0.0f && zEntry < 0.0f || xEntry > 1.0f || yEntry > 1.0f || zEntry > 1.0f)
	{
		// No collision occured
	}
	else
	{
		__debugbreak();
	}
}
