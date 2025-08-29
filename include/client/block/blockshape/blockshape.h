#pragma once

#include <glm/glm.hpp>
#include <vector>

enum BlockShapeType : unsigned char
{
	CUBE,
	CROSS,
	SLAB_BOTTOM,
	SLAB_TOP,
};

struct BlockShapeFace
{
	float offsetX, offsetY, offsetZ;
	float scaleX, scaleY;
	float rotationX, rotationY;
	float uvOffsetX, uvOffsetY;
	float uvScaleX, uvScaleY;

	int textureId;
	bool cullable;
	glm::ivec3 faceDirection;
};

class BlockShape
{
public:
	BlockShape(BlockShapeType shapeType);

	const std::vector<BlockShapeFace>& getFaces() const;
	BlockShapeType getShapeType() const;
protected:
	BlockShapeType shapeType;
	std::vector<BlockShapeFace> faces;
};
