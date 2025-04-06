#pragma once

#include "shader.h"
#include "texture.h"
#include <unordered_map>

class ResourceManager
{
public:
	ResourceManager();

	Texture& getTexture(const std::string& texturePath);
	Shader& getShader(const std::string& shaderPath);
private:
	std::unordered_map<std::string, Shader> loadedShaders;
	std::unordered_map<std::string, Texture> loadedTextures;
};