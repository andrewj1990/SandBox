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
	m_CollisionBox = BoundingBox(position.x, position.y, size.x, size.y);
	//m_Occluder = std::shared_ptr<BoundingBox>(new BoundingBox(position.x, position.y, size.x, size.y));
}

void Sprite::init()
{
	m_Angle = 0.0f;
	m_Solid = true;
}

void Sprite::setPosition(float x, float y)
{
	int offsetX = m_CollisionBox.x - m_Position.x;
	int offsetY = m_CollisionBox.y - m_Position.y;

	m_Position.x = x;
	m_Position.y = y;

	m_CollisionBox.x = x + offsetX;
	m_CollisionBox.y = y + offsetY;
}

void Sprite::fade(float fadeAmount)
{
	m_Colour.w -= fadeAmount;

	if (m_Colour.w < 0) {
		m_Colour.w = 0;
	}

	//if (m_Colour.w <= 0.1) m_Destroy = true;
}