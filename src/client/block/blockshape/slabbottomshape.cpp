#include "block/blockshape/slabbottomshape.h"

SlabBottomShape::SlabBottomShape(int frontFaceTexId, int rightFaceTexId, int backFaceTexId, int leftFaceTexId, int topFaceTexId, int bottomFaceTexId)
	: BlockShape(BlockShapeType::SLAB_BOTTOM)
{
	BlockShapeFace frontFace = BlockShapeFace
	{
		0.5f, 0.25f, 1.0f, // Offset
		1.0f, 0.5f, // Scale
		0.0f, 0.0f, // Rotation

		0.0f, 0.0f, // UV Offset
		1.0f, 0.5f, // UV Scale

		frontFaceTexId, // Texture
		true, // Cullable
		glm::ivec3(0, 0, 1), // Normal
	};

	BlockShapeFace rightFace = BlockShapeFace
	{
		1.0f, 0.25f, 0.5f, // Offset
		1.0f, 0.5f, // Scale
		0.0f, -90.0f, // Rotation

		0.0f, 0.0f, // UV Offset
		1.0f, 0.5f, // UV Scale

		rightFaceTexId, // Texture
		true, // Cullable
		glm::ivec3(1, 0, 0), // Normal
	};

	BlockShapeFace backFace = BlockShapeFace
	{
		0.5f, 0.25f, 0.0f, // Offset
		1.0f, 0.5f, // Scale
		0.0f, 180.0f, // Rotation

		0.0f, 0.0f, // UV Offset
		1.0f, 0.5f, // UV Scale

		backFaceTexId, // Texture
		true, // Cullable
		glm::ivec3(0, 0, -1), // Normal
	};

	BlockShapeFace leftFace = BlockShapeFace
	{
		0.0f, 0.25f, 0.5f, // Offset
		1.0f, 0.5f, // Scale
		0.0f, 90.0f, // Rotation

		0.0f, 0.0f, // UV Offset
		1.0f, 0.5f, // UV Scale

		leftFaceTexId, // Texture
		true, // Cullable
		glm::ivec3(-1, 0, 0), // Normal
	};

	BlockShapeFace topFace = BlockShapeFace
	{
		0.5f, 0.5f, 0.5f, // Offset
		1.0f, 1.0f, // Scale
		90.0f, 0.0f, // Rotation

		0.0f, 0.0f, // UV Offset
		1.0f, 1.0f, // UV Scale

		topFaceTexId, // Texture
		false, // Cullable
		glm::ivec3(0, 1, 0), // Normal
	};

	BlockShapeFace bottomFace = BlockShapeFace
	{
		0.5f, 0.0f, 0.5f, // Offset
		1.0f, 1.0f, // Scale
		-90.0f, 0.0f, // Rotation

		0.0f, 0.0f, // UV Offset
		1.0f, 1.0f, // UV Scale

		bottomFaceTexId, // Texture
		true, // Cullable
		glm::ivec3(0, -1, 0), // Normal
	};

	faces.push_back(frontFace);
	faces.push_back(rightFace);
	faces.push_back(backFace);
	faces.push_back(leftFace);
	faces.push_back(topFace);
	faces.push_back(bottomFace);
}
