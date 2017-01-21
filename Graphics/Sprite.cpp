#include "sprite.h"

Sprite::Sprite()
	: Renderable()
{
	m_Angle = 0.0f;
}

Sprite::Sprite(const glm::vec3& position, const glm::vec2 size, const glm::vec4& colour)
	: Renderable(position, size, colour)
{
	m_Angle = 0.0f;
}

Sprite::Sprite(const glm::vec3& position, const glm::vec2 size, Texture* texture)
	: Renderable(position, size)
{
	m_Angle = 0.0f;
	m_Texture = texture;
}

void Sprite::setPosition(float x, float y)
{
	m_Position.x = x;
	m_Position.y = y;
}

void Sprite::fade(float fadeAmount)
{
	m_Colour.w -= fadeAmount;
}