#include "Bullet.h"

Bullet::Bullet(float x, float y, float angle)
	: Entity(glm::vec3(x, y, 500), glm::vec2(4, 2), TextureManager::get("Textures/Bullet2.png"))
{
	m_Angle = angle;

	float velocity = 5000;
	m_Dx = std::cosf(angle) * velocity;
	m_Dy = std::sinf(angle) * velocity;

	m_Duration = 2.0f;

	m_Light = Sprite(glm::vec3(0, 0, 0), glm::vec2(32, 32), TextureManager::get("Textures/light2.png"));
}

void Bullet::update(float timeElapsed)
{
	// bullet tracer effect
	if (m_Destroy)
	{
		m_Size.x -= 30;
		return;
	}
	m_Size.x += 30;

	m_Duration -= timeElapsed;


	addDirection(m_Dx * timeElapsed, m_Dy * timeElapsed);

	// use tail end of bullet trail if shooting downwards
	if (glm::degrees(m_Angle) < 0) m_Position.z = -m_Position.y - m_Size.x;

	m_CollisionBox->x = getPosition().x;
	m_CollisionBox->y = getPosition().y;

	if (m_Duration <= 0)
	{
		m_Destroy = true;
	}

	m_Light.setPosition(getCenterX() - m_Light.getSize().x / 2, getCenterY() - m_Light.getSize().y / 2);
}

void Bullet::submit(Renderer& renderer)
{
	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(m_Position.x, m_Position.y + m_Size.y / 2.0f, 0));
	transform = glm::rotate(transform, m_Angle + glm::radians(180.0f), glm::vec3(0, 0, 1));
	transform = glm::translate(transform, glm::vec3(-m_Position.x, -m_Position.y - m_Size.y / 2.0f, 0));
	renderer.push(transform);
	renderer.submit(*this);
	renderer.pop();
}

void Bullet::renderLight(Renderer& renderer)
{
	renderer.submit(m_Light);
}
