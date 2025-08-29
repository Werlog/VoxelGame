#include "block/blockshape/shapespecification.h"

BlockShapeSpecification BlockShapeSpecification::convertBlockShape(BlockShape& blockShape)
{
	BlockShapeSpecification shape = BlockShapeSpecification{};

	shape.type = blockShape.getShapeType();

	const std::vector<BlockShapeFace>& faces = blockShape.getFaces();

	for (size_t i = 0; i < std::min((int)faces.size(), 10); i++)
	{
		shape.data.faces[i].offsetX = faces[i].offsetX;
		shape.data.faces[i].offsetY = faces[i].offsetY;
		shape.data.faces[i].offsetZ = faces[i].offsetZ;

		shape.data.faces[i].scaleX = faces[i].scaleX;
		shape.data.faces[i].scaleY = faces[i].scaleY;

		shape.data.faces[i].rotationX = faces[i].rotationX;
		shape.data.faces[i].rotationY = faces[i].rotationY;

		shape.data.faces[i].uvOffsetX = faces[i].uvOffsetX;
		shape.data.faces[i].uvOffsetY = faces[i].uvOffsetY;

		shape.data.faces[i].uvScaleX = faces[i].uvScaleX;
		shape.data.faces[i].uvScaleY = faces[i].uvScaleY;

		shape.data.faces[i].normalX = faces[i].faceDirection.x;
		shape.data.faces[i].normalY = faces[i].faceDirection.y;
		shape.data.faces[i].normalZ = faces[i].faceDirection.z;
	}

	return shape;
}
