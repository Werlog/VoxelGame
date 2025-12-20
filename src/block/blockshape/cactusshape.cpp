#include "block/blockshape/cactusshape.h"

CactusShape::CactusShape(int sideTextureId, int topTextureId, int bottomTextureId)
	: BlockShape(BlockShapeType::CACTUS_SHAPE)
{
	BlockShapeFace frontFace = BlockShapeFace
	{
		0.5f, 0.5f, 0.9375f, // Offset
		1.0f, 1.0f, // Scale
		0.0f, 0.0f, // Rotation

		0.0f, 0.0f, // UV Offset
		1.0f, 1.0f, // UV Scale

		sideTextureId, // Texture
		false, // Cullable
		glm::ivec3(0, 0, 1), // Normal
	};

	BlockShapeFace rightFace = BlockShapeFace
	{
		0.9375f, 0.5f, 0.5f, // Offset
		1.0f, 1.0f, // Scale
		0.0f, 90.0f, // Rotation

		0.0f, 0.0f, // UV Offset
		1.0f, 1.0f, // UV Scale

		sideTextureId, // Texture
		false, // Cullable
		glm::ivec3(1, 0, 0), // Normal
	};

	BlockShapeFace backFace = BlockShapeFace
	{
		0.5f, 0.5f, 0.0625f, // Offset
		1.0f, 1.0f, // Scale
		0.0f, 180.0f, // Rotation

		0.0f, 0.0f, // UV Offset
		1.0f, 1.0f, // UV Scale

		sideTextureId, // Texture
		false, // Cullable
		glm::ivec3(0, 0, -1), // Normal
	};

	BlockShapeFace leftFace = BlockShapeFace
	{
		0.0625f, 0.5f, 0.5f, // Offset
		1.0f, 1.0f, // Scale
		0.0f, -90.0f, // Rotation

		0.0f, 0.0f, // UV Offset
		1.0f, 1.0f, // UV Scale

		sideTextureId, // Texture
		false, // Cullable
		glm::ivec3(-1, 0, 0), // Normal
	};

	BlockShapeFace topFace = BlockShapeFace
	{
		0.5f, 1.0f, 0.5f, // Offset
		1.0f, 1.0f, // Scale
		-90.0f, 0.0f, // Rotation

		0.0f, 0.0f, // UV Offset
		1.0f, 1.0f, // UV Scale

		topTextureId, // Texture
		true, // Cullable
		glm::ivec3(0, 1, 0), // Normal
	};

	BlockShapeFace bottomFace = BlockShapeFace
	{
		0.5f, 0.0f, 0.5f, // Offset
		1.0f, 1.0f, // Scale
		90.0f, 0.0f, // Rotation

		0.0f, 0.0f, // UV Offset
		1.0f, 1.0f, // UV Scale

		bottomTextureId, // Texture
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
