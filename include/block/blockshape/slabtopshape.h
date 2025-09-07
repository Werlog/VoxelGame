#pragma once

#include "blockshape.h"

class SlabTopShape : public BlockShape
{
public:
	SlabTopShape(int frontFaceTexId, int rightFaceTexId, int backFaceTexId, int leftFaceTexId, int topFaceTexId, int bottomFaceTexId);
};
