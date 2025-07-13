#pragma once

#include "blockdata.h"
#include <unordered_map>

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
private:
	// Maps block types to OpenGL texture handles
	std::unordered_map<BlockType, unsigned int> iconMap;

	unsigned int blockVAO;
	unsigned int blockVBO;
	unsigned int blockEBO;

	unsigned int FBO;
	unsigned int depthAttachmentHandle;
	
	void setupBlock();
};
