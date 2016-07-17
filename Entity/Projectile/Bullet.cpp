#include "Bullet.h"

Bullet::Bullet(float x, float y, float angle)
	: Entity(0, 0)
{
	m_Sprite = Sprite(glm::vec3(x, y, 0), glm::vec2(32, 32), TextureManager::get("Textures/Player/Bullet.png"));

	float velocity = 1000;
	m_Dx = std::cosf(angle) * velocity;
	m_Dy = std::sinf(angle) * velocity;

	m_Duration = 2.0f;
}

void Bullet::update(float timeElapsed)
{
	m_Duration -= timeElapsed;

	m_Sprite.addDirection(m_Dx * timeElapsed, m_Dy * timeElapsed);

	if (m_Duration <= 0)
	{
		m_Destroy = true;
	}

}

void Bullet::render(Renderer& renderer)
{
	renderer.render(m_Sprite);
}