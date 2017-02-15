#include "sprite.h"

Sprite::Sprite()
	: Renderable()
{
	init();
}

Sprite::Sprite(const glm::vec3& position, const glm::vec2 size, const glm::vec4& colour)
	: Renderable(position, size, colour)
{
	init();
}

Sprite::Sprite(const glm::vec3& position, const glm::vec2 size, Texture* texture)
	: Renderable(position, size, texture)
{
	init();
}

void Sprite::init()
{
	m_Angle = 0.0f;
	m_Solid = false;
}

void Sprite::setPosition(float x, float y)
{
	m_Position.x = x;
	m_Position.y = y;
}

void Sprite::fade(float fadeAmount)
{
	m_Colour.w -= fadeAmount;
	if (m_Colour.w <= 0) m_Destroy = true;
}