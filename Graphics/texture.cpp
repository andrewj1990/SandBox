#include "texture.h"

Texture::Texture(unsigned int width, unsigned int height, GLuint tid)
	: m_Width(width), m_Height(height), m_TID(tid)
{
	setDefaultUV();
}

Texture::Texture(const std::string& filename)
	: m_FileName(filename), m_xIndex(0), m_yIndex(0)
{
	m_TID = load();
	setDefaultUV();
}

void Texture::setDefaultUV()
{
	m_UV.push_back(glm::vec4(0, 0, 0, 0));
	m_UV.push_back(glm::vec4(0, 1, 0, 0));
	m_UV.push_back(glm::vec4(1, 1, 0, 0));
	m_UV.push_back(glm::vec4(1, 0, 0, 0));
}

void Texture::setUV(float ix, float iy, float w, float h, float sw, float sh)
{
	float tx = (ix * w) / sw;
	float ty = (iy * h) / sh;
	float tw = (w / sw);
	float th = (h / sh);

	m_UV.push_back(glm::vec4(tx, ty, w, h));
	m_UV.push_back(glm::vec4(tx, ty + th, w, h));
	m_UV.push_back(glm::vec4(tx + tw, ty + th, w, h));
	m_UV.push_back(glm::vec4(tx + tw, ty, w, h));
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_TID);
}

GLuint Texture::load()
{
	GLubyte* textura = loadImage(m_FileName.c_str(), m_Width, m_Height);

	GLuint result;
	glGenTextures(1, &result);
	glBindTexture(GL_TEXTURE_2D, result);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)textura);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//GLfloat fLargest;
	//glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);

	delete textura;
	return result;
}

void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_TID);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
