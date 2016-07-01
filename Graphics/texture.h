#pragma once

#include "../Utils/loadImage.h"
#include <GL\glew.h>
#include <string>
#include <vector>
#include <glm\glm.hpp>
#include "spritesheet.h"

class SpriteSheet;

class Texture
{
public:
	
	Texture(const std::string& filename);
	Texture(SpriteSheet& spritesheet, const std::string& filename, int xIndex, int yIndex, int width, int height);
	~Texture();

	void setDefaultUV();

	const std::string& getName() const { return m_FileName; }
	inline const GLuint& getTID() const { return m_TID; }
	
	void setUV(float ix, float iy, float w, float h, float sw, float sh);
	inline const std::vector<glm::vec2> getUVs() { return m_UV; }
	const int& getWidth() { return m_Width; }
	const int& getHeight() { return m_Height; }
	const int& getIndexX() { return m_xIndex; }
	const int& getIndexY() { return m_yIndex; }

	void bind() const;
	void unbind() const;

private:
	GLuint load();

private:
	std::string m_FileName;
	GLuint m_TID;
	unsigned int m_Width;
	unsigned int m_Height;
	std::vector<glm::vec2> m_UV;

	int m_xIndex;
	int m_yIndex;

};