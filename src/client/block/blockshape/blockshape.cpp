#include "block/blockshape/blockshape.h"

BlockShape::BlockShape(BlockShapeType shapeType)
{
	this->shapeType = shapeType;
}

const std::vector<BlockShapeFace>& BlockShape::getFaces() const
{
	return this->faces;
}

BlockShapeType BlockShape::getShapeType() const
{
	return shapeType;
}
