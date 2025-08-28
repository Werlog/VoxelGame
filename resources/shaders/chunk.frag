#version 330 core

in vec3 texCoord;
in vec3 worldPosition;
flat in uint lightLevel;
in float brightness;

out vec4 outColor;

uniform sampler2DArray terrainTexture;
uniform vec3 cameraPosition;

uniform float fogStart = 150.0f;
uniform float fogEnd = 325.0f;
uniform vec3 fogColor = vec3(0.1f, 0.62f, 1.0f);
uniform vec3 minLightLevel = vec3(0.5f, 0.5f, 0.5f);


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

	vec3 lightMultiplier = mix(minLightLevel, vec3(1), lightLevel / 15.0f);

	vec4 texColor = texture(terrainTexture, texCoord);

	if (texColor.a < 0.2f)
		discard;

	outColor = mix(vec4(fogColor, 1.0f), vec4((texColor * brightness).xyz * lightMultiplier, 1.0f), fogFactor);
}
