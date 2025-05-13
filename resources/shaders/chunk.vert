#version 430

uniform float texUnitX;
uniform float texUnitY;

uniform int atlasSizeX;
uniform int atlasSizeY;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 lightDirection;

struct FaceData
{
	uint data;
	uint biomeData;
};

layout(std430, binding = 0) readonly buffer vertexPullBuffer
{
	FaceData meshData[];
};

const vec3 vertexPositions[24] = vec3[24]
(
	// Front Face
	vec3(0, 0, 0),
	vec3(1, 0, 0),
	vec3(0, 1, 0),
	vec3(1, 1, 0),

	// Right Face
	vec3(1, 0, 0),
	vec3(1, 0, -1),
	vec3(1, 1, 0),
	vec3(1, 1, -1),

	// Back Face
	vec3(1, 0, -1),
	vec3(0, 0, -1),
	vec3(1, 1, -1),
	vec3(0, 1, -1),

	// Left Face
	vec3(0, 0, -1),
	vec3(0, 0, 0),
	vec3(0, 1, -1),
	vec3(0, 1, 0),

	// Top Face
	vec3(0, 1, 0),
	vec3(1, 1, 0),
	vec3(0, 1, -1),
	vec3(1, 1, -1),

	// Bottom Face
	vec3(0, 0, 0),
	vec3(0, 0, -1),
	vec3(1, 0, 0),
	vec3(1, 0, -1)
);

int indices[36] = int[36]
(
	// Front Face
	0, 1, 2,
	1, 3, 2,

	// Right Face
	4, 5, 6,
	5, 7, 6,

	// Back Face
	8, 9, 10,
	9, 11, 10,

	// Left Face
	12, 13, 14,
	13, 15, 14,

	// Top Face
	16, 17, 18,
	17, 19, 18,

	// Bottom Face
	20, 21, 22,
	21, 23, 22
);

const vec2 uvs[4] = vec2[4]
(
	vec2(0.0f, 0.0f),
	vec2(1.0f, 0.0f),
	vec2(0.0f, 1.0f),
	vec2(1.0f, 1.0f)
);

vec3 voxelNormals[6] = vec3[6](
	vec3(0, 0, 1),
	vec3(1, 0, 0),
	vec3(0, 0, -1),
	vec3(-1, 0, 0),
	vec3(0, 1, 0),
	vec3(0, -1, 0)
);

vec3 biomeColors[1] = vec3[1](
	vec3(0.55f, 0.95f, 0.35f)
);

out vec2 texCoord;
out vec3 colorMod;
out vec3 worldPosition;
flat out uint lightLevel;
out float brightness;

bool shouldUseBiomeColor(uint faceMask, uint faceDirection)
{
	switch (faceDirection)
	{
		case 0:
			return (faceMask & 0x01) != 0;
		case 1:
			return (faceMask & 0x02) != 0;
		case 2:
			return (faceMask & 0x04) != 0;
		case 3:
			return (faceMask & 0x08) != 0;
		case 4:
			return (faceMask & 0x10) != 0;
		case 5:
			return (faceMask & 0x20) != 0;
		default:
			return false;
	}
}

void main()
{
	int index = gl_VertexID / 6;
	uint data = meshData[index].data;
	uint biomeData = meshData[index].biomeData;
	int curVertexId = gl_VertexID % 6;

	uint xPos = data & 63;
	uint yPos = (data >> 6) & 63;
	uint zPos = (data >> 12) & 63;
	uint textureId = (data >> 18) & 255;
	uint faceDirection = (data >> 26) & 7;

	uint biomeColorIndex = biomeData & 15;
	uint faceMask = (biomeData >> 4) & 63;
	lightLevel = (biomeData >> 10) & 15;

	vec3 position = vec3(xPos, yPos, zPos);
	int vertexIndex = curVertexId + 6 * int(faceDirection);

	position += vertexPositions[indices[vertexIndex]];

	uint row = textureId / atlasSizeX;
	uint column = textureId - row * atlasSizeX;
	if (row == 0)
		column = textureId;

	float rowPos = 1 - texUnitY - (row / float(atlasSizeY));
	float columnPos = column / float(atlasSizeX);

	vec2 uvs = vec2(uvs[indices[curVertexId]].x, uvs[indices[curVertexId]].y);

	texCoord = vec2(columnPos, rowPos) + vec2(uvs.x * texUnitX, uvs.y * texUnitY);

	vec3 normal = voxelNormals[faceDirection];

	brightness = max(dot(normal, lightDirection), -0.5f);
	brightness += 0.5f;
	brightness = clamp(brightness, 0.65f, 1.0f);

	// projection * view * model

	gl_Position = projection * view * model * vec4(position, 1.0f);
	worldPosition = (model * vec4(position, 1.0f)).xyz;

	colorMod = vec3(1);

	if (biomeColorIndex < biomeColors.length() && shouldUseBiomeColor(faceMask, faceDirection))
	{
		colorMod = biomeColors[biomeColorIndex];
	}
}
