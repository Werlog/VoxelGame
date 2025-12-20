#include "block/blockshape/panerightshape.h"

PaneRightShape::PaneRightShape(int textureId)
	: BlockShape(BlockShapeType::PANE_RIGHT)
{
	BlockShapeFace frontFace = BlockShapeFace
	{
		0.5f, 0.5f, 1.0f, // Offset
		0.1f, 1.0f, // Scale
		0.0f, 0.0f, // Rotation

		0.0f, 0.0f, // UV Offset
		0.1f, 1.0f, // UV Scale

		textureId, // Texture
		true, // Cullable
		glm::ivec3(0, 0, 1), // Normal
	};

	BlockShapeFace rightFace = BlockShapeFace
	{
		0.55f, 0.5f, 0.5f, // Offset
		1.0f, 1.0f, // Scale
		0.0f, 90.0f, // Rotation

		0.0f, 0.0f, // UV Offset
		1.0f, 1.0f, // UV Scale

		textureId, // Texture
		false, // Cullable
		glm::ivec3(1, 0, 0), // Normal
	};

	BlockShapeFace backFace = BlockShapeFace
	{
		0.5f, 0.5f, 0.0f, // Offset
		0.1f, 1.0f, // Scale
		0.0f, 180.0f, // Rotation

		0.0f, 0.0f, // UV Offset
		0.1f, 1.0f, // UV Scale

		textureId, // Texture
		true, // Cullable
		glm::ivec3(0, 0, -1), // Normal
	};

	BlockShapeFace leftFace = BlockShapeFace
	{
		0.45f, 0.5f, 0.5f, // Offset
		1.0f, 1.0f, // Scale
		0.0f, -90.0f, // Rotation

		0.0f, 0.0f, // UV Offset
		1.0f, 1.0f, // UV Scale

		textureId, // Texture
		false, // Cullable
		glm::ivec3(-1, 0, 0), // Normal
	};

	BlockShapeFace topFace = BlockShapeFace
	{
		0.5f, 1.0f, 0.5f, // Offset
		0.1f, 1.0f, // Scale
		-90.0f, 0.0f, // Rotation

		0.0f, 0.0f, // UV Offset
		0.1f, 1.0f, // UV Scale

		textureId, // Texture
		true, // Cullable
		glm::ivec3(0, 1, 0), // Normal
	};

	BlockShapeFace bottomFace = BlockShapeFace
	{
		0.5f, 0.0f, 0.5f, // Offset
		0.1f, 1.0f, // Scale
		90.0f, 0.0f, // Rotation

		0.0f, 0.0f, // UV Offset
		0.1f, 1.0f, // UV Scale

		textureId, // Texture
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
