#pragma once

#include <glm/glm.hpp>

namespace math
{
	float lerp(float a, float b, float t);
	glm::vec3 lerp(glm::vec3 a, glm::vec3 b, float t);

	float clamp(float val, float min, float max);

	float cubicEase(float t);
}
