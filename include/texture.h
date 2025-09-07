#pragma once

#include <string>

class Texture
{
public:
	Texture(const std::string& texturePath);
	~Texture();

	unsigned int getTextureHandle() const;
	int getWidth() const;
	int getHeight() const;
private:
	unsigned int textureHandle;
	int width, height;

	void loadTexture(const std::string& texturePath);
};