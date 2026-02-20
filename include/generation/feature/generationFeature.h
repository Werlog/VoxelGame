#pragma once

#include "AABB.h"
#include <memory>

class Chunk;

class GenerationFeature
{
public:
	
	GenerationFeature(glm::vec3 origin);

	const AABB& getBoundingBox() const;

	virtual void generate(std::shared_ptr<Chunk> chunk) const = 0;
protected:
	AABB boundingBox;
	glm::vec3 origin;
};
