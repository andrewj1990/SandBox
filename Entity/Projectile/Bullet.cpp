#include "Bullet.h"

Bullet::Bullet(float x, float y, float angle)
	: Entity(0, 0)
{
	m_Sprite = Sprite(glm::vec3(x, y, 0), glm::vec2(32, 32), TextureManager::get("Textures/Player/Bullet.png"));
	m_Dx = std::cosf(angle);
	m_Dy = std::sinf(angle);
}

void Bullet::update(float timeElapsed)
{
	m_Sprite.addDirection(m_Dx, m_Dy);
}

void Bullet::render(Renderer& renderer)
{
	renderer.render(m_Sprite);
}