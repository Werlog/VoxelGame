#pragma once

#include "shader.h"
#include "texture.h"
#include "UI/font.h"
#include <unordered_map>

class ResourceManager
{
public:
	ResourceManager();

	Texture& getTexture(const std::string& texturePath);
	Shader& getShader(const std::string& shaderPath);
	Font& getFont(const std::string& fontPath);
private:
	std::unordered_map<std::string, Shader> loadedShaders;
	std::unordered_map<std::string, Texture> loadedTextures;
	std::unordered_map<std::string, Font> loadedFonts;
};
