#include "UI/font.h"
#include <glad/glad.h>
#include <iostream>

Font::Font(const std::string& path)
{
	load(path);
}

const Character& Font::getCharacter(char c)
{
	return characters.at(c);
}

void Font::load(const std::string& path)
{
	std::string fontPath = std::string(RESOURCES_PATH + path);

	FT_Library lib;
	if (FT_Init_FreeType(&lib) != 0)
	{
		std::cout << "Failed to initialize FreeType" << std::endl;
		return;
	}

	FT_Face face;
	if (FT_New_Face(lib, fontPath.c_str(), 0, &face) != 0)
	{
		std::cout << "Failed to load font at " << fontPath << std::endl;
		FT_Done_FreeType(lib);
		return;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned char c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER) != 0)
		{
			std::cout << "Failed to load character " << int(c) << std::endl;
			continue;
		}

		unsigned int tex;
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = Character
		{
			tex,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(unsigned int)face->glyph->advance.x
		};
		characters.insert({ c, character });
	}

	FT_Done_Face(face);
	FT_Done_FreeType(lib);
}
