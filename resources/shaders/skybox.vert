#version 430

layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;

out vec3 vDir;

void main()
{
	gl_Position = projection * mat4(mat3(view)) * vec4(aPos, 1.0f);
	vDir = aPos;
}