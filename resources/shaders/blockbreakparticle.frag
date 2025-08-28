#version 430

in vec3 texCoord;

out vec4 aColor;

uniform sampler2DArray blockTexture;

void main()
{
	aColor = texture(blockTexture, texCoord);

	if (aColor.a < 0.1f)
		discard;
}
