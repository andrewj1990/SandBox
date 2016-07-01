#pragma once

#include "texture.h"
#include "texture_manager.h"
#include <memory>

class Texture;

class SpriteSheet
{
public:
	SpriteSheet(const std::string& filepath);

	void add(const std::string& name, int xIndex, int yIndex, int width, int height);
	Texture& get(int xIndex, int yIndex);

	const std::unique_ptr<Texture>& getTexture() { return m_Texture; }
	//Texture& getTexture() { return m_Texture; }
	const GLuint& getTID() { return m_TID; }
	const int& getWidth() { return m_Width; }
	const int& getHeight() { return m_Height; }

private:
	std::string m_SpritesheetName;
	std::unique_ptr<Texture> m_Texture;
	//Texture m_Texture;
	GLuint m_TID;
	unsigned int m_Width;
	unsigned int m_Height;

};