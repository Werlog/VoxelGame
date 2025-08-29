#include "block/blockshape/crossshape.h"

CrossShape::CrossShape(int textureId)
	: BlockShape(BlockShapeType::CROSS)
{
	BlockShapeFace sideOneFront = BlockShapeFace
	{
		0.5f, 0.4f, 0.5f, // Offset
		0.9f, 0.8f, // Scale
		0.0f, 45.0f, // Rotation

		0.0f, 0.0f, // UV Offset
		1.0f, 1.0f, // UV Scale

		textureId, // Texture
		false, // Cullable
		glm::ivec3(0, 1, 0), // Normal
	};

	BlockShapeFace sideTwoFront = BlockShapeFace
	{
		0.5f, 0.4f, 0.5f, // Offset
		0.9f, 0.8f, // Scale
		0.0f, 315.0f, // Rotation

		0.0f, 0.0f, // UV Offset
		1.0f, 1.0f, // UV Scale

		textureId, // Texture
		false, // Cullable
		glm::ivec3(0, 1, 0), // Normal
	};

	BlockShapeFace sideOneBack = BlockShapeFace
	{
		0.5f, 0.4f, 0.5f, // Offset
		-0.9f, 0.8f, // Scale
		0.0f, 45.0f, // Rotation

		0.0f, 0.0f, // UV Offset
		-1.0f, 1.0f, // UV Scale

		textureId, // Texture
		false, // Cullable
		glm::ivec3(0, 1, 0), // Normal
	};

	BlockShapeFace sideTwoBack = BlockShapeFace
	{
		0.5f, 0.4f, 0.5f, // Offset
		-0.9f, 0.8f, // Scale
		0.0f, 315.0f, // Rotation

		0.0f, 0.0f, // UV Offset
		-1.0f, 1.0f, // UV Scale

		textureId, // Texture
		false, // Cullable
		glm::ivec3(0, 1, 0), // Normal
	};

	faces.push_back(sideOneFront);
	faces.push_back(sideTwoFront);
	faces.push_back(sideOneBack);
	faces.push_back(sideTwoBack);
}
