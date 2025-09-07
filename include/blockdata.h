#pragma once

#include <unordered_map>
#include <memory>
#include "block/block.h"
#include "block/blockshape/shapespecification.h"
#include "shader.h"

class BlockData
{
public:

	BlockData(Shader& chunkShader);

	const std::shared_ptr<Block>& getBlock(BlockType blockType);

	const std::unordered_map<BlockType, std::shared_ptr<Block>>& getData() const;
	
	int getShapeIndex(BlockShapeType shapeType) const;

	unsigned int getShapeSSBO() const;
private:
	unsigned int shapeSSBO;

	std::unordered_map<BlockType, std::shared_ptr<Block>> data;

	// Maps Shape types to indexes of the GPU-side shape array
	std::unordered_map<BlockShapeType, int> shapeIndexMap;

	void initShapes();
	void uploadShapes(const std::vector<BlockShapeSpecification>& shapes);
	void init();
};
