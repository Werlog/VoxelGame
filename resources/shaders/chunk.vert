#version 430

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 lightDirection;

struct FaceData
{
	uint data;
	uint secondData;
};

struct BlockShapeFace
{
	mat4 transform;

	vec2 uvOffset;
	vec2 uvScale;

	vec3 normal;
};

struct BlockShape
{
	BlockShapeFace faces[10];
};

layout(std430, binding = 1) readonly buffer voxelShapeBuffer
{
	BlockShape shapes[];
};

layout(std430, binding = 0) readonly buffer vertexPullBuffer
{
	FaceData meshData[];
};

const vec3 vertexPositions[4] = vec3[4]
(
	vec3(-0.5f, -0.5f, 0.0f),
	vec3(0.5f, -0.5f, 0.0f),
	vec3(-0.5f, 0.5f, 0.0f),
	vec3(0.5f, 0.5f, 0.0f)
);

int indices[6] = int[6]
(
	0, 1, 2,
	1, 3, 2
);

const vec2 uvs[4] = vec2[4]
(
	vec2(0.0f, 0.0f),
	vec2(1.0f, 0.0f),
	vec2(0.0f, 1.0f),
	vec2(1.0f, 1.0f)
);

out vec3 texCoord;
out vec3 worldPosition;
flat out uint lightLevel;
out float brightness;

void main()
{
	int index = gl_VertexID / 6;
	uint data = meshData[index].data;
	uint secondData = meshData[index].secondData;

	int curVertexIndex = gl_VertexID % 6;

	uint xPos = data & 63;
	uint yPos = (data >> 6) & 63;
	uint zPos = (data >> 12) & 63;
	uint textureId = (data >> 18) & 255;
	lightLevel = (data >> 26) & 15;

	uint shapeIndex = secondData & 15;
	uint faceIndex = (secondData >> 4) & 15;
	uint ao = (secondData >> 8) & 255;

	const BlockShapeFace face = shapes[shapeIndex].faces[faceIndex];

	vec2 uvScale = face.uvScale;
	vec2 uvOffset = face.uvOffset;
	vec3 normal = face.normal;

	vec4 modelPos = face.transform * vec4(vertexPositions[indices[curVertexIndex]], 1.0f);
	vec3 position = modelPos.xyz;

	position += vec3(xPos, yPos, zPos);

	texCoord = vec3(uvs[indices[curVertexIndex]].x * uvScale.x + uvOffset.x, uvs[indices[curVertexIndex]].y * uvScale.y + uvOffset.y, textureId);

	brightness = max(dot(normal, lightDirection), -0.5f);
	brightness += 0.55f;
	brightness = clamp(brightness, 0.65f, 1.0f);
	brightness = 1.0f;
	if (ao != 0)
	{
		uint aoIndex = indices[curVertexIndex] * 2;
		uint vertexAO = (ao >> aoIndex) & 3;
		brightness -= vertexAO * 0.25f;
	}

	gl_Position = projection * view * model * vec4(position, 1.0f);
	worldPosition = (model * vec4(position, 1.0f)).xyz;
}
