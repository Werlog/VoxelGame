#pragma once

#include <string>

class TextureSheet
{
public:
	TextureSheet(int textureWidth, int textureHeight, const std::string& sheetPath);

	unsigned int getTextureHandle() const;
private:
	int textureWidth, textureHeight;
	unsigned int textureHandle;

	void setup(const std::string& sheetPath);
};
