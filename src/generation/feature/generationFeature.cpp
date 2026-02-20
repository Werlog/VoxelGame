#include "generation/feature/generationFeature.h"

GenerationFeature::GenerationFeature(glm::vec3 origin)
{
	this->origin = origin;
}

const AABB& GenerationFeature::getBoundingBox() const
{
	return boundingBox;
}
