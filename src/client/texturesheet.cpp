#include "texturesheet.h"

TextureSheet::TextureSheet(int textureWidth, int textureHeight, Texture* texture)
{
	this->textureWidth = textureWidth;
	this->textureHeight = textureHeight;
	this->sheet = texture;
}

const Texture& TextureSheet::getSheet() const
{
	return *sheet;
}

float TextureSheet::getOneUnitX() const
{
	return textureWidth / (float)sheet->getWidth();
}

float TextureSheet::getOneUnitY() const
{
	return textureHeight / (float)sheet->getHeight();
}
