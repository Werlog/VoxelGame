#version 430

in vec3 texCoord;
in vec3 colorMultiplier;
in float brightness;

uniform sampler2DArray blockTexture;

out vec4 aColor;

void main()
{
	vec4 color = texture(blockTexture, texCoord);
	aColor = vec4(color.xyz * brightness * colorMultiplier, color.a);

	if (aColor.a < 0.2f)
		discard;
}
