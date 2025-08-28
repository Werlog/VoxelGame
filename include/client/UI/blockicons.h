#pragma once

#include "blockdata.h"
#include <unordered_map>
#include "resourcemanager.h"

constexpr int blockIconResolution = 256;

class BlockIcons
{
public:

	BlockIcons(ResourceManager& resourceManager);

	void init(BlockData& blockData);

	unsigned int getTextureForBlock(BlockType block) const;

	const std::unordered_map<BlockType, unsigned int>& getIconMap() const;
private:
	// Maps block types to OpenGL texture handles
	std::unordered_map<BlockType, unsigned int> iconMap;

	Shader& chunkShader;

	unsigned int blockVAO;
	unsigned int blockSSBO;

	unsigned int FBO;
	unsigned int depthAttachmentHandle;

	// Uniforms
	unsigned int viewLoc;
	unsigned int modelLoc;
	unsigned int projectionLoc;
	unsigned int lightDirLoc;
	
	void setupBlock();
	void initUniforms();
	int updateBlockSSBO(std::shared_ptr<Block> newBlock, BlockData& blockData);
};
