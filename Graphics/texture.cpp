#include "texture.h"

Texture::Texture(const std::string& filename)
	: m_FileName(filename), m_xIndex(0), m_yIndex(0)
{
	m_TID = load();
	setDefaultUV();
}

Texture::Texture(SpriteSheet& spritesheet, const std::string& filename, int xIndex, int yIndex, int width, int height)
	: m_FileName(filename), m_TID(spritesheet.getTID()), m_xIndex(xIndex), m_yIndex(yIndex)
{
	float inverseIndex = ((float)spritesheet.getHeight() / (float)height) - (float)yIndex - 1;
	setUV((float)xIndex, inverseIndex, (float)width, (float)height, (float)spritesheet.getWidth(), (float)spritesheet.getHeight());
}

void Texture::setDefaultUV()
{
	m_UV.push_back(glm::vec2(0, 0));
	m_UV.push_back(glm::vec2(0, 1));
	m_UV.push_back(glm::vec2(1, 1));
	m_UV.push_back(glm::vec2(1, 0));
}

void Texture::setUV(float ix, float iy, float w, float h, float sw, float sh)
{
	float tx = (ix * w) / sw;
	float ty = (iy * h) / sh;
	float tw = (w / sw);
	float th = (h / sh);

	m_UV.push_back(glm::vec2(tx, ty));
	m_UV.push_back(glm::vec2(tx, ty + th));
	m_UV.push_back(glm::vec2(tx + tw, ty + th));
	m_UV.push_back(glm::vec2(tx + tw, ty));
}

Texture::~Texture()
{

}

GLuint Texture::load()
{
	//BYTE* pixels = loadImage(m_FileName.c_str(), m_Width, m_Height);

	//GLuint result;
	//glGenTextures(1, &result);
	//glBindTexture(GL_TEXTURE_2D, result);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_BGR, GL_UNSIGNED_BYTE, pixels);
	//glBindTexture(GL_TEXTURE_2D, 0);

	//delete[] pixels;
	//return result;

	//const char* textureFile = m_FileName.c_str();
	//FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(textureFile, 0);//Automatocally detects the format(from over 20 formats!)
	//FIBITMAP* imagen = FreeImage_Load(formato, textureFile);

	//FIBITMAP* temp = imagen;
	//imagen = FreeImage_ConvertTo32Bits(imagen);
	//FreeImage_Unload(temp);

	//int w = FreeImage_GetWidth(imagen);
	//int h = FreeImage_GetHeight(imagen);

	//GLubyte* textura = new GLubyte[4 * w*h];
	//char* pixeles = (char*)FreeImage_GetBits(imagen);
	////FreeImage loads in BGR format, so you need to swap some bytes(Or use GL_BGR).

	//for (int j = 0; j<w*h; j++) {
	//	textura[j * 4 + 0] = pixeles[j * 4 + 2];
	//	textura[j * 4 + 1] = pixeles[j * 4 + 1];
	//	textura[j * 4 + 2] = pixeles[j * 4 + 0];
	//	textura[j * 4 + 3] = pixeles[j * 4 + 3];
	//	//cout<<j<<": "<<textura[j*4+0]<<"**"<<textura[j*4+1]<<"**"<<textura[j*4+2]<<"**"<<textura[j*4+3]<<endl;
	//}

	//Now generate the OpenGL texture object 

	GLubyte* textura = loadImage(m_FileName.c_str(), m_Width, m_Height);

	GLuint result;
	glGenTextures(1, &result);
	glBindTexture(GL_TEXTURE_2D, result);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)textura);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

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
