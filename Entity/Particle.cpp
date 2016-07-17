#include "Particle.h"

Particle::Particle(float x, float y)
	: Entity(glm::vec3(x, y, 0.0f), glm::vec2(5, 5), glm::vec4(1, 0, 0, rand() % 1000 / 1000.0f))
{
	m_Dx = 1.0f - (rand() % 1000) / 500.0f;
	m_Dy = 1.0f - (rand() % 1000) / 500.0f;

	float vel = 64.0f;
	m_Dx *= vel;
	m_Dy *= vel;

	m_Duration = 0.2f + (rand() % 1000) / 1000.0f;
}

Particle::Particle(float x, float y, float angle)
	: Entity(glm::vec3(x, y, 0.0f), glm::vec2(5, 5), glm::vec4(1, 0, 0, rand() % 1000 / 1000.0f))
{
	float angleOffset = 50.0f - (rand() % 1000 / 10.0f);

	float velX = 20.0f + (rand() % 1000 / 10.0f);
	float velY = 20.0f + (rand() % 1000 / 10.0f);
	float a = angle + glm::radians(angleOffset);

	m_Dx = std::cosf(a) * velX;
	m_Dy = std::sinf(a) * velY;

	m_Duration = 0.2f + (rand() % 1000) / 1000.0f;
}

void Particle::update(float timeElapsed)
{
	addDirection(m_Dx * timeElapsed, m_Dy * timeElapsed);

	m_Duration -= timeElapsed;

	if (m_Duration <= 0)
	{
		m_Destroy = true;
	}
}

void Particle::render(Renderer& renderer)
{
	renderer.render(*this);
}
