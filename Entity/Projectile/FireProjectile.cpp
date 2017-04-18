#include "FireProjectile.h"

FireProjectile::FireProjectile(float x, float y, float angleRad)
	: Entity(glm::vec3(x - 16, y - 16, 0), glm::vec2(32, 32), TextureManager::get("Textures/Mobs/fireball.png"))
{
	m_Angle = angleRad;

	float velocity = 500;//5000;
	m_Dx = std::cosf(angleRad) * velocity;
	m_Dy = std::sinf(angleRad) * velocity;

	m_Duration = 2.0f;

	m_Light = Sprite(glm::vec3(0, 0, 0), glm::vec2(4, 6), TextureManager::get("Textures/Bullet2.png"));
}

void FireProjectile::update(float timeElapsed)
{
	// bullet tracer effect
	m_Duration -= timeElapsed;

	addDirection(m_Dx * timeElapsed, m_Dy * timeElapsed);

	m_CollisionBox->x = getPosition().x;
	m_CollisionBox->y = getPosition().y;

	if (m_Duration <= 0)
	{
		m_Destroy = true;
	}

	m_Light.setSize(glm::vec2(m_Size.x, m_Light.getSize().y));
	m_Light.setLightPosition(getX(), getY(), m_Size.x / 2);
	m_Light.setPosition(getCenterX() - m_Light.getSize().x / 2, getCenterY() - m_Light.getSize().y / 2);
}

void FireProjectile::submit(Renderer& renderer)
{
	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(m_Position.x, m_Position.y + m_Size.y / 2.0f, 0));
	transform = glm::rotate(transform, m_Angle, glm::vec3(0, 0, 1));
	transform = glm::translate(transform, glm::vec3(-m_Position.x, -m_Position.y - m_Size.y / 2.0f, 0));
	renderer.push(transform);
	renderer.submit(*this);
	renderer.pop();
}

void FireProjectile::renderLight(Renderer& renderer)
{
	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(m_Position.x, m_Position.y + m_Size.y / 2.0f, 0));
	transform = glm::rotate(transform, m_Angle, glm::vec3(0, 0, 1));
	transform = glm::translate(transform, glm::vec3(-m_Position.x, -m_Position.y - m_Size.y / 2.0f, 0));
	renderer.push(transform);
	renderer.submit(m_Light);
	renderer.pop();
}
