#version 330 core

in vec2 texCoord;

uniform sampler2D terrainTexture;

out vec4 outColor;

void main()
{
	outColor = texture(terrainTexture, texCoord);

	if (outColor.a < 0.1f)
		discard;
}