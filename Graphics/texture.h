#pragma once

#include "../Utils/loadImage.h"
#include <GL\glew.h>
#include <string>
#include <vector>
#include <glm\glm.hpp>

class Texture
{
public:
	
	Texture(const std::string& filename);
	~Texture();

	void setDefaultUV();

	const std::string& getName() const { return m_FileName; }
	inline const GLuint& getTID() const { return m_TID; }
	
	void setUV(float ix, float iy, float w, float h, float sw, float sh);
	inline const std::vector<glm::vec4> getUVs() { return m_UV; }
	int getWidth() { return m_Width; }
	int getHeight() { return m_Height; }
	int getIndexX() { return m_xIndex; }
	int getIndexY() { return m_yIndex; }

	void bind() const;
	void unbind() const;

private:
	GLuint load();

private:
	std::string m_FileName;
	GLuint m_TID;
	unsigned int m_Width;
	unsigned int m_Height;
	std::vector<glm::vec4> m_UV;

	int m_xIndex;
	int m_yIndex;

};