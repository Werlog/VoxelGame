#include "resourcemanager.h"

ResourceManager::ResourceManager()
{

}

Texture& ResourceManager::getTexture(const std::string& texturePath)
{
	auto it = loadedTextures.find(texturePath);
	if (it != loadedTextures.end())
		return it->second;

	auto inserted = loadedTextures.try_emplace(texturePath, texturePath);
	
	return inserted.first->second;
}

Shader& ResourceManager::getShader(const std::string& shaderPath)
{
	auto it = loadedShaders.find(shaderPath);
	if (it != loadedShaders.end())
		return it->second;

	auto inserted = loadedShaders.try_emplace(shaderPath, shaderPath + ".vert", shaderPath + ".frag");

	return inserted.first->second;
}
