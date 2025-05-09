#version 430

in vec2 texCoord;
out vec4 aColor;

uniform sampler2D tex;

void main()
{
	aColor = texture(tex, texCoord);
}
