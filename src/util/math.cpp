#include "util/math.h"
#include "math.h"
#include <algorithm>

float math::lerp(float a, float b, float t)
{
	return (1.0f - t) * a + t * b;
}

glm::vec3 math::lerp(glm::vec3 a, glm::vec3 b, float t)
{
	return glm::vec3(lerp(a.x, b.x, t), lerp(a.y, b.y, t), lerp(a.z, b.z, t));
}

float math::clamp(float val, float min, float max)
{
	return std::max(std::min(val, max), min);
}
