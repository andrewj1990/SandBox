#pragma once

#include <freetype-gl.h>
#include <string>

class Font
{
public:
	Font(std::string name, std::string filename = "LuckiestGuy.ttf", int size = 14);

	ftgl::texture_font_t* getFTFont() const { return m_FTFont; }

	const std::string& getName() const { return m_Name; }
	const std::string& getFilename() const { return m_Filename; }
	const unsigned int& getSize() const { return m_Size; }
	const unsigned int getID() const { return m_FTAtlas->id; }

private:
	ftgl::texture_atlas_t* m_FTAtlas;
	ftgl::texture_font_t* m_FTFont;

	unsigned int m_Size;

	std::string m_Name;
	std::string m_Filename;

};