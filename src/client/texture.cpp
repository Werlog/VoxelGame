#include "texture.h"
#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include <iostream>

Texture::Texture(const std::string& texturePath)
{
	textureHandle = 0;
	width = 0;
	height = 0;

	loadTexture(texturePath);
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureHandle);
}

void Texture::loadTexture(const std::string& texturePath)
{
	if (textureHandle != 0) return;

	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_2D, textureHandle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(1);

	int nrChannels;
	unsigned char* data = stbi_load(std::string(RESOURCES_PATH + texturePath).c_str(), &width, &height, &nrChannels, 0);
	if (data && (nrChannels == 3 || nrChannels == 4))
	{
		int format = nrChannels == 3 ? GL_RGB : GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture " << texturePath << std::endl;
	}

	stbi_image_free(data);
}

unsigned int Texture::getTextureHandle() const
{
	return textureHandle;
}

int Texture::getWidth() const
{
	return width;
}

int Texture::getHeight() const
{
	return height;
}
