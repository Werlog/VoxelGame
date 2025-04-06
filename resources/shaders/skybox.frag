#version 430

in vec3 vDir;
out vec4 aColor;

uniform vec3 topSkyColor;
uniform vec3 bottomSkyColor;

void main()
{
	float t = normalize(vDir).y * 0.5f + 0.5f;
	aColor = vec4(mix(bottomSkyColor, topSkyColor, t), 1.0f);
}
