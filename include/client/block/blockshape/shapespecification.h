#pragma once

#include "blockshape.h"

// GPU-side Face definition
struct BlockFaceSpecification
{
	float offsetX, offsetY, offsetZ;
	float scaleX, scaleY;
	float rotationX, rotationY;
	float uvOffsetX, uvOffsetY;

	float normalX, normalY, normalZ;
};

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
