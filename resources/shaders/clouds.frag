#version 430

uniform sampler2D cloudTex;

in vec2 texCoord;

out vec4 aColor;

void main()
{
	aColor = texture(cloudTex, texCoord);
	if (aColor.a > 0.1f)
		aColor.a = 0.5f;
}
