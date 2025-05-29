#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <unordered_map>
#include <string>

struct Character
{
	unsigned int texID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;
};

class Font
{
public:

	Font(const std::string& path);

	const Character& getCharacter(char c);
private:
	std::unordered_map<char, Character> characters;

	void load(const std::string& path);
};
