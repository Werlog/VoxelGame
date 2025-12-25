#include "viewmodel/viewmotion.h"

ViewMotion::ViewMotion()
{
	this->offset = glm::vec3(0.0f);
	this->weight = 1.0f;
}

const glm::vec3& ViewMotion::getOffset() const
{
	return offset;
}

float ViewMotion::getWeight() const
{
	return weight;
}
