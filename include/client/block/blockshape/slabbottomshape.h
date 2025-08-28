#pragma once

#include "blockshape.h"

class SlabBottomShape : public BlockShape
{
public:
	SlabBottomShape(int frontFaceTexId, int rightFaceTexId, int backFaceTexId, int leftFaceTexId, int topFaceTexId, int bottomFaceTexId);
};
