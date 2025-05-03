#version 430

out vec4 aColor;

uniform vec3 lineColor;

void main()
{
	aColor = vec4(lineColor, 1.0f);
}
