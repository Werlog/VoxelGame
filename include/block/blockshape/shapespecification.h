#pragma once

#include "blockshape.h"
#include <glm/glm.hpp>

// GPU-side Face definition
struct alignas(16) BlockFaceSpecification
{
	glm::mat4 transform;

	glm::vec2 uvOffset;
	glm::vec2 uvScale;

	glm::vec3 normal;
	float _pad2;
};

static_assert(sizeof(BlockFaceSpecification) == 96, "BlockFaceSpecification must be 96 bytes");

struct BlockShapeSpecification
{
	struct BlockShapeData
	{
		BlockFaceSpecification faces[10];
	};

	BlockShapeType type;
	BlockShapeData data;

	static BlockShapeSpecification convertBlockShape(BlockShape& blockShape);
};
