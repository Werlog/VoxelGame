#include "texturesheet.h"
#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include <iostream>

TextureSheet::TextureSheet(int textureWidth, int textureHeight, const std::string& sheetPath)
{
	this->textureWidth = textureWidth;
	this->textureHeight = textureHeight;
	this->textureHandle = 0;

	setup(sheetPath);
}

unsigned int TextureSheet::getTextureHandle() const
{
	return textureHandle;
}

void TextureSheet::setup(const std::string& sheetPath)
{
	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureHandle);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(1);

	int sheetWidth, sheetHeight;
	int nrChannels;
	unsigned char* sheetData = stbi_load(std::string(RESOURCES_PATH + sheetPath).c_str(), &sheetWidth, &sheetHeight, &nrChannels, 4);

	int texturesX = sheetWidth / textureWidth;
	int texturesY = sheetHeight / textureHeight;
	int layers = texturesX * texturesY;

	if (sheetData && nrChannels == 4)
	{
		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, textureWidth, textureHeight, layers, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		for (int y = 0; y < texturesY; y++)
		{
			for (int x = 0; x < texturesX; x++)
			{
				int layer = x + (texturesY - y - 1) * texturesX;

				unsigned char* layerData = new unsigned char[textureWidth * textureHeight * nrChannels];

				for (int row = 0; row < textureHeight; row++)
				{
					int sheetOffset = (y * textureHeight + row) * sheetWidth + x * textureWidth;
					std::memcpy(layerData + row * textureWidth * nrChannels, sheetData + sheetOffset * nrChannels, textureWidth * nrChannels);
				}

				glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, textureWidth, textureHeight, 1, GL_RGBA, GL_UNSIGNED_BYTE, layerData);

				delete[] layerData;
			}
		}
	}
	else
	{
		std::cout << "Failed to load texture sheet " << sheetPath << std::endl;
	}

	stbi_image_free(sheetData);
}
