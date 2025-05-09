#version 430

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aUV;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 playerPosition;
uniform float time;

uniform float planeSize; // How big is one pixel in world units

out vec2 texCoord;

void main()
{
	float height = 120.0f - playerPosition.y;

	gl_Position = projection * mat4(mat3(view)) * vec4(aPosition.x, height, aPosition.z, 1.0f);

	vec2 offset = vec2(playerPosition.x / planeSize, (playerPosition.z / planeSize) + time * (1.0f / planeSize));

	texCoord = (aUV + offset) * 0.25f;
}
