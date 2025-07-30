#version 430

// Per vertex
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

// Per instance
layout (location = 2) in vec4 transform;
layout (location = 3) in vec2 uvOffset;

out vec3 texCoord;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 camRight;
uniform vec3 camUp;
uniform int blockType;

void main()
{
	float scale = transform.w;
	vec3 instancePos = transform.xyz;
	vec3 vertexPos = instancePos + aPos.x * scale * camRight + aPos.y * scale * camUp;

	gl_Position = projection * view * vec4(vertexPos, 1.0f);
	texCoord = vec3((aUV * scale) + uvOffset, blockType);
}
