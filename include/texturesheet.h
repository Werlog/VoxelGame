#include "texture.h"

class TextureSheet
{
public:
	TextureSheet(int textureWidth, int textureHeight, const std::string& texturePath);

	const Texture& getSheet() const;

	float getOneUnitX() const;
	float getOneUnitY() const;

private:
	Texture sheet;

	int textureWidth, textureHeight;
};
