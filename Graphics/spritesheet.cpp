#include "spritesheet.h"

SpriteSheet::SpriteSheet(const std::string& filepath)
	: m_SpritesheetName(filepath)
{
	m_Texture = std::unique_ptr<Texture>(new Texture(filepath));
	m_TID = m_Texture->getTID();
	m_Width = m_Texture->getWidth();
	m_Height = m_Texture->getHeight();
}

void SpriteSheet::add(const std::string& name, int xIndex, int yIndex, int width, int height)
{
	TextureManager::add(new Texture(*this, name, xIndex, yIndex, width, height));
}

Texture& SpriteSheet::get(int xIndex, int yIndex)
{
	return *m_Texture;
}