#pragma once

#include "blockdata.h"
#include <unordered_map>
#include "shader.h"

constexpr int blockIconResolution = 256;

struct IconVertex
{
	float x, y, z;
	float u, v;
	int faceId;
};

class BlockIcons
{
public:

	BlockIcons();

	void init(BlockData& blockData);

	unsigned int getTextureForBlock(BlockType block) const;

	const std::unordered_map<BlockType, unsigned int>& getIconMap() const;
private:
	// Maps block types to OpenGL texture handles
	std::unordered_map<BlockType, unsigned int> iconMap;

	unsigned int blockVAO;
	unsigned int blockVBO;
	unsigned int blockEBO;

	unsigned int FBO;
	unsigned int depthAttachmentHandle;

	// Uniforms
	unsigned int viewLoc;
	unsigned int modelLoc;
	unsigned int projectionLoc;

	unsigned int lightDirLoc;
	unsigned int biomeMaskLoc;

	unsigned int frontFaceIdLoc;
	unsigned int rightFaceIdLoc;
	unsigned int backFaceIdLoc;
	unsigned int leftFaceIdLoc;
	unsigned int topFaceIdLoc;
	unsigned int bottomFaceIdLoc;
	
	void setupBlock();

	void initUniforms(Shader& iconShader);

	void setUniforms(const BlockProperties& properties);
};
