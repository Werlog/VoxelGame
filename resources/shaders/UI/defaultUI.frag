#version 430

in vec2 texCoord;
out vec4 aColor;

uniform bool textureAssigned;
uniform sampler2D tex;
uniform vec4 color;

void main()
{
	if (textureAssigned)
		aColor = texture(tex, texCoord);
	else
		aColor = color;

	if (aColor.a < 0.1f && textureAssigned)
		discard;
}
