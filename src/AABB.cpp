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

bool AABB::isOverlapping(const AABB& other) const
{
	const glm::vec3& otherMin = other.getMin();
	const glm::vec3& otherMax = other.getMax();

	return min.x < otherMax.x && max.x > otherMin.x
		&& min.y < otherMax.y && max.y > otherMin.y
		&& min.z < otherMax.z && max.z > otherMin.z;
}

CollisionResult AABB::collide(const AABB& other, const glm::vec3& velocity)
{
	const glm::vec3& otherMin = other.getMin();
	const glm::vec3& otherMax = other.getMax();

	// Checks for collision on non-moving axis, eliminates false positives.
	auto discreteCheck = [this, velocity, other, otherMin, otherMax]() -> bool
		{
			if (velocity.x == 0.0f && (min.x > otherMax.x || max.x < otherMin.x))
			{
				return false;
			}
			if (velocity.y == 0.0f && (min.y > otherMax.y || max.y < otherMin.y))
			{
				return false;
			}
			
			if (velocity.z == 0.0f && (min.z > otherMax.z || max.z < otherMin.z))
			{
				return false;
			}
			return true;
		};

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
		return CollisionResult
		{
			1.0f,
			glm::vec3(0),
		};
	}
	else
	{
		if (!discreteCheck())
		{
			return CollisionResult
			{
				1.0f,
				glm::vec3(0),
			};
		}

		glm::vec3 normal = glm::vec3(0);
		if (xEntry > yEntry && xEntry > zEntry)
		{
			normal = velocity.x > 0 ? glm::vec3(-1.0f, 0.0f, 0.0f) : glm::vec3(1.0f, 0.0f, 0.0f);
		}
		else if (yEntry > xEntry && yEntry > zEntry)
		{
			normal = velocity.y > 0 ? glm::vec3(0.0f, -1.0f, 0.0f) : glm::vec3(0.0f, 1.0f, 0.0f);
		}
		else
		{
			normal = velocity.z > 0 ? glm::vec3(0.0f, 0.0f, -1.0f) : glm::vec3(0.0f, 0.0f, 1.0f);
		}

		return CollisionResult
		{
			entryTime,
			normal,
		};
	}
}
