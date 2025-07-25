#version 430

layout (location = 0) in vec4 vertex;

out vec2 texCoord;

uniform mat4 projection;
uniform mat4 model;
uniform vec2 uvScaler;

void main()
{
	gl_Position = projection * model * vec4(vertex.xy, 0.0f, 1.0f);
	texCoord = vertex.zw * uvScaler;
}
