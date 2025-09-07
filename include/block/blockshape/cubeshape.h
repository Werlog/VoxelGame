#pragma once

#include "blockshape.h"

class CubeShape : public BlockShape
{
public:
	CubeShape(int frontFaceTexId, int rightFaceTexId, int backFaceTexId, int leftFaceTexId, int topFaceTexId, int bottomFaceTexId);
};
