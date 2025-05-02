#version 330 core

in vec2 texCoord;

uniform sampler2D terrainTexture;
uniform vec3 cameraPosition;

uniform float fogStart = 75.0f;
uniform float fogEnd = 325.0f;
uniform vec3 fogColor = vec3(0.2f, 0.8f, 1.0f);

out vec4 outColor;
in vec3 worldPosition;
in float brightness;

float calcLinearFogFactor()
{
	float pixelDist = length(worldPosition - cameraPosition);
	float fogRange = fogEnd - fogStart;
	float fogDist = fogEnd - pixelDist;
	float fogFactor = fogDist / fogRange;

	return clamp(fogFactor, 0.0f, 1.0f);
}

void main()
{
	float fogFactor = calcLinearFogFactor();

	outColor = mix(vec4(fogColor, 1.0f), vec4((texture(terrainTexture, texCoord) * brightness).xyz, 1.0f), fogFactor);

	if (outColor.a < 0.1f)
		discard;
}
