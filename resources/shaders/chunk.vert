#version 430

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 lightDirection;

struct FaceData
{
	uint data;
	uint secondData;
};

struct BlockShapeFace
{
	float offsetX, offsetY, offsetZ;
	float scaleX, scaleY;
	float rotationX, rotationY;
	float uvOffsetX, uvOffsetY;

	float normalX, normalY, normalZ;
};

struct BlockShape
{
	BlockShapeFace faces[10];
};

layout(std430, binding = 1) readonly buffer voxelShapeBuffer
{
	BlockShape shapes[];
};

layout(std430, binding = 0) readonly buffer vertexPullBuffer
{
	FaceData meshData[];
};

const vec3 vertexPositions[4] = vec3[4]
(
	vec3(-0.5f, -0.5f, 0.0f),
	vec3(0.5f, -0.5f, 0.0f),
	vec3(-0.5f, 0.5f, 0.0f),
	vec3(0.5f, 0.5f, 0.0f)
);

int indices[6] = int[6]
(
	0, 1, 2,
	1, 3, 2
);

const vec2 uvs[4] = vec2[4]
(
	vec2(0.0f, 0.0f),
	vec2(1.0f, 0.0f),
	vec2(0.0f, 1.0f),
	vec2(1.0f, 1.0f)
);

mat3 rotationMatrixNoZ(vec2 eulerXY) {
    float cx = cos(eulerXY.x); // pitch
    float sx = sin(eulerXY.x);
    float cy = cos(eulerXY.y); // yaw
    float sy = sin(eulerXY.y);

    // Rotation order Y * X (yaw then pitch)
    return mat3(
        cy,      0.0,      sy,
        sx*sy,   cx,      -sx*cy,
       -cx*sy,   sx,       cx*cy
    );
}

out vec3 texCoord;
out vec3 worldPosition;
flat out uint lightLevel;
out float brightness;

void main()
{
	int index = gl_VertexID / 6;
	uint data = meshData[index].data;
	uint secondData = meshData[index].secondData;

	int curVertexIndex = gl_VertexID % 6;

	uint xPos = data & 63;
	uint yPos = (data >> 6) & 63;
	uint zPos = (data >> 12) & 63;
	uint textureId = (data >> 18) & 255;
	lightLevel = (data >> 26) & 15;

	uint shapeIndex = secondData & 15;
	uint faceIndex = (secondData >> 4) & 15;

	const BlockShape shape = shapes[shapeIndex];
	const float RAD2DEG = 3.14159265 / 180.0;

	vec3 scale = vec3(shape.faces[faceIndex].scaleX, shape.faces[faceIndex].scaleY, 1.0f);
	vec3 position = vertexPositions[indices[curVertexIndex]] * scale;

	vec3 offset = vec3(shape.faces[faceIndex].offsetX, shape.faces[faceIndex].offsetY, shape.faces[faceIndex].offsetZ);
	vec2 euler = vec2(shape.faces[faceIndex].rotationX, shape.faces[faceIndex].rotationY) * RAD2DEG;
	position = rotationMatrixNoZ(euler) * position;

	position += vec3(xPos, yPos, zPos) + offset;

	texCoord = vec3(uvs[indices[curVertexIndex]].x * scale.x, uvs[indices[curVertexIndex]].y * scale.y, textureId);

	vec3 normal = vec3(shape.faces[faceIndex].normalX, shape.faces[faceIndex].normalY, shape.faces[faceIndex].normalZ);

	brightness = max(dot(normal, lightDirection), -0.5f);
	brightness += 0.5f;
	brightness = clamp(brightness, 0.65f, 1.0f);

	gl_Position = projection * view * model * vec4(position, 1.0f);
	worldPosition = (model * vec4(position, 1.0f)).xyz;
}
