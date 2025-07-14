#version 430

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in int faceId;

out vec3 texCoord;
out float brightness;
out vec3 colorMultiplier;

uniform vec3 lightDirection;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;
uniform int frontFaceId;
uniform int rightFaceId;
uniform int backFaceId;
uniform int leftFaceId;
uniform int topFaceId;
uniform int bottomFaceId;
uniform int biomeMask;

vec3 voxelNormals[6] = vec3[6](
	vec3(0, 0, 1),
	vec3(1, 0, 0),
	vec3(0, 0, -1),
	vec3(-1, 0, 0),
	vec3(0, 1, 0),
	vec3(0, -1, 0)
);

int getLayer()
{
	switch (faceId)
	{
		case 0:
			return frontFaceId;
		case 1:
			return rightFaceId;
		case 2:
			return backFaceId;
		case 3:
			return leftFaceId;
		case 4:
			return topFaceId;
		case 5:
			return bottomFaceId;
		default:
			return frontFaceId;
	}
}

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
	gl_Position = projection * view * model * vec4(aPos, 1.0f);

	vec3 normal = voxelNormals[faceId];

	brightness = max(dot(normal, lightDirection), -0.5f);
	brightness += 0.5f;
	brightness = clamp(brightness, 0.65f, 1.0f);

	texCoord = vec3(aUV, getLayer());

	if (shouldUseBiomeColor(biomeMask, faceId))
		colorMultiplier = vec3(0.55f, 0.95f, 0.35f);
	else
		colorMultiplier = vec3(1.0f);
}
