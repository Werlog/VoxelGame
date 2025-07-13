#include "UI/blockicons.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "camera.h"

BlockIcons::BlockIcons()
{
	this->FBO = 0;
	this->depthAttachmentHandle = 0;

	this->blockVAO = 0;
	this->blockVBO = 0;
	this->blockEBO = 0;
}

void BlockIcons::init(BlockData& blockData)
{
    setupBlock();

    Camera camera = Camera(glm::vec3(1.9f, 1.9f, -1.9f), 45.0f, 1.0f);

    Shader iconShader = Shader("shaders\\blockIcon.vert", "shaders\\blockIcon.frag");
    glUseProgram(iconShader.getProgramHandle());

    glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 viewMatrix = glm::lookAt(camera.getPosition(), glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f));

    unsigned int viewLoc = glGetUniformLocation(iconShader.getProgramHandle(), "view");
    unsigned int modelLoc = glGetUniformLocation(iconShader.getProgramHandle(), "model");
    unsigned int projectionLoc = glGetUniformLocation(iconShader.getProgramHandle(), "projection");
    unsigned int texIdLoc = glGetUniformLocation(iconShader.getProgramHandle(), "textureId");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &depthAttachmentHandle);
	glBindTexture(GL_TEXTURE_2D, depthAttachmentHandle);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, blockIconResolution, blockIconResolution);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthAttachmentHandle, 0);

    const std::unordered_map<BlockType, BlockProperties>& blocks = blockData.getData();

    for (auto it = blocks.begin(); it != blocks.end(); it++)
    {
        BlockType type = it->first;

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
			__debugbreak();
		}

        // Render the icon
		glViewport(0, 0, blockIconResolution, blockIconResolution);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		const BlockProperties& properties = blockData.getBlockProperties(type);

        glUniform1i(texIdLoc, (int)properties.frontFaceTexId);
        glDisable(GL_CULL_FACE);
        glBindVertexArray(blockVAO);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glEnable(GL_CULL_FACE);

		iconMap.insert({ type, iconTexture });
    }

	glDeleteBuffers(1, &blockVBO);
	glDeleteBuffers(1, &blockEBO);
	glDeleteVertexArrays(1, &blockVAO);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int BlockIcons::getTextureForBlock(BlockType block) const
{
	auto it = iconMap.find(block);
	if (it == iconMap.end())
		return 0;
	return it->second;
}

void BlockIcons::setupBlock()
{
	constexpr float vertices[] =
	{
		// Front Face
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,

		// Right Face
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, -1.0f, 1.0f, 1.0f,

		// Back Face
		1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, -1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, -1.0f, 1.0f, 1.0f,

		// Left Face
		0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
		0.0f, 1.0f, -1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,  1.0f, 1.0f,

		// Top Face
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, -1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, -1.0f, 1.0f, 1.0f,

		// Bottom Face
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, -1.0f, 1.0f, 1.0f,
	};

	constexpr int indices[] =
	{
		// Front Face
		0, 1, 2,
		1, 3, 2,

		// Right Face
		4, 5, 6,
		5, 7, 6,

		// Back Face
		8, 9, 10,
		9, 11, 10,

		// Left Face
		12, 13, 14,
		13, 15, 14,

		// Top Face
		16, 17, 18,
		17, 19, 18,

		// Bottom Face
		20, 21, 22,
		21, 23, 22,
	};

    glGenVertexArrays(1, &blockVAO);
    glBindVertexArray(blockVAO);

    glGenBuffers(1, &blockVBO);
    glGenBuffers(1, &blockEBO);

    glBindBuffer(GL_ARRAY_BUFFER, blockVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, blockEBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}
