#pragma once

#include <glm/glm.hpp>
#include "AABB.h"

struct FrustumPlane
{
	glm::vec3 normal;
	float distance;

	void normalize()
	{
		float length = glm::length(normal);
		normal /= length;
		distance /= length;
	}

	float distanceToPoint(const glm::vec3& point) const
	{
		return glm::dot(normal, point) + distance;
	}

	bool isOnOrForwardPlane(AABB& aabb) const
	{
		const glm::vec3& max = aabb.getMax();
		glm::vec3 positiveVertex = aabb.getMin();

		if (normal.x > 0) positiveVertex.x = max.x;
		if (normal.y > 0) positiveVertex.y = max.y;
		if (normal.z > 0) positiveVertex.z = max.z;

		if (distanceToPoint(positiveVertex) < 0.0f)
		{
			return false;
		}

		return true;
	}
};

struct Frustum
{
	FrustumPlane nearPlane;
	FrustumPlane farPlane;
	FrustumPlane rightPlane;
	FrustumPlane leftPlane;
	FrustumPlane topPlane;
	FrustumPlane bottomPlane;

	void normalize()
	{
		nearPlane.normalize();
		farPlane.normalize();
		leftPlane.normalize();
		rightPlane.normalize();
		topPlane.normalize();
		bottomPlane.normalize();
	}
};
