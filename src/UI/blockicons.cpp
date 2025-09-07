#include "UI/blockicons.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"
#include <iostream>
#include "chunk.h"

BlockIcons::BlockIcons(ResourceManager& resourceManager)
	: chunkShader(resourceManager.getShader("shaders/chunk"))
{
	this->FBO = 0;
	this->depthAttachmentHandle = 0;

	this->blockVAO = 0;
	this->blockSSBO = 0;

	this->viewLoc = 0;
	this->modelLoc = 0;
	this->projectionLoc = 0;
	this->lightDirLoc = 0;
}

void BlockIcons::init(BlockData& blockData)
{
    setupBlock();

    Camera camera = Camera(glm::vec3(1.6f, 2.0f, 2.0f), 45.0f, 1.0f);

	glm::vec3 lightDirection = glm::normalize(glm::vec3(0.5f, 1.0f, 1.0f));

    glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 viewMatrix = glm::lookAt(camera.getPosition(), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f));

	initUniforms();

	glUseProgram(chunkShader.getProgramHandle());

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDirection));

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &depthAttachmentHandle);
	glBindTexture(GL_TEXTURE_2D, depthAttachmentHandle);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, blockIconResolution, blockIconResolution);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthAttachmentHandle, 0);

    const std::unordered_map<BlockType, std::shared_ptr<Block>>& blocks = blockData.getData();

    for (auto it = blocks.begin(); it != blocks.end(); it++)
    {
        BlockType type = it->first;
		const std::shared_ptr<Block>& block = it->second;

        if (type == BlockType::AIR) // pointless
            continue;

        unsigned int iconTexture;
        glGenTextures(1, &iconTexture);
        glBindTexture(GL_TEXTURE_2D, iconTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, blockIconResolution, blockIconResolution);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, iconTexture, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "FAILED TO CREATE FRAME BUFFER FOR THE ICON OF BLOCK \"" << block->getName() << "\"" << std::endl;
			continue;
		}

		int faceCount = updateBlockSSBO(block, blockData);

        // Render the icon
		glViewport(0, 0, blockIconResolution, blockIconResolution);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(chunkShader.getProgramHandle());

        glBindVertexArray(blockVAO);

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, blockSSBO);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, blockData.getShapeSSBO());

		glDrawArrays(GL_TRIANGLES, 0, faceCount * 6);

		glUseProgram(0);
        glBindVertexArray(0);

		iconMap.insert({ type, iconTexture });
    }

	glDeleteBuffers(1, &blockSSBO);
	glDeleteVertexArrays(1, &blockVAO);
	glDeleteTextures(1, &depthAttachmentHandle);
	glDeleteFramebuffers(1, &FBO);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int BlockIcons::getTextureForBlock(BlockType block) const
{
	auto it = iconMap.find(block);
	if (it == iconMap.end())
		return 0;
	return it->second;
}

const std::unordered_map<BlockType, unsigned int>& BlockIcons::getIconMap() const
{
	return iconMap;
}

void BlockIcons::setupBlock()
{
	glGenVertexArrays(1, &blockVAO);
	glGenBuffers(1, &blockSSBO);
}

void BlockIcons::initUniforms()
{
	viewLoc = glGetUniformLocation(chunkShader.getProgramHandle(), "view");
	modelLoc = glGetUniformLocation(chunkShader.getProgramHandle(), "model");
	projectionLoc = glGetUniformLocation(chunkShader.getProgramHandle(), "projection");
	lightDirLoc = glGetUniformLocation(chunkShader.getProgramHandle(), "lightDirection");
}

int BlockIcons::updateBlockSSBO(std::shared_ptr<Block> block, BlockData& blockData)
{
	std::shared_ptr<BlockShape> shape = block->getBlockShape(block->getBlockType());
	const std::vector<BlockShapeFace>& faces = shape->getFaces();

	std::vector<ChunkFace> blockFaces = std::vector<ChunkFace>();
	blockFaces.reserve(faces.size());

	for (size_t i = 0; i < faces.size(); i++)
	{
		const BlockShapeFace& face = faces[i];

		constexpr int lightLevel = 15;

		int shapeIndex = blockData.getShapeIndex(shape->getShapeType());

		blockFaces.push_back(ChunkFace{ (face.textureId << 18 | lightLevel << 26), (shapeIndex | (int)i << 4 )});
	}

	glBindVertexArray(blockVAO);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, blockSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, blockFaces.size() * sizeof(ChunkFace), blockFaces.data(), GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);

	return blockFaces.size();
}
