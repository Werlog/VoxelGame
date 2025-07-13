#version 430

in vec3 texCoord;

uniform sampler2DArray blockTexture;

out vec4 aColor;

void main()
{
	aColor = texture(blockTexture, texCoord);

	if (aColor.a < 0.2f)
		discard;
}
