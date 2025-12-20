#include "block/blockshape/shapespecification.h"
#include <glm/gtc/matrix_transform.hpp>

BlockShapeSpecification BlockShapeSpecification::convertBlockShape(BlockShape& blockShape)
{
	BlockShapeSpecification shape = BlockShapeSpecification{};

	shape.type = blockShape.getShapeType();

	const std::vector<BlockShapeFace>& faces = blockShape.getFaces();

	for (size_t i = 0; i < std::min((int)faces.size(), 10); i++)
	{
		const BlockShapeFace& face = faces[i];

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(face.offsetX, face.offsetY, face.offsetZ));
		transform = glm::rotate(transform, glm::radians(face.rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
		transform = glm::rotate(transform, glm::radians(face.rotationY), glm::vec3(0.0f, 1.0f, 0.0f));

		transform = glm::scale(transform, glm::vec3(face.scaleX, face.scaleY, 1.0f));

		shape.data.faces[i].transform = transform;
		shape.data.faces[i].uvOffset = glm::vec2(face.uvOffsetX, face.uvOffsetY);
		shape.data.faces[i].uvScale = glm::vec2(face.uvScaleX, face.uvScaleY);
		shape.data.faces[i].normal = glm::vec3(face.faceDirection);
	}

	return shape;
}
