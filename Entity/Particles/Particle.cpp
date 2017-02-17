#include "Particle.h"

Particle::Particle(float x, float y, float size, float angleDeg)
	: Entity(glm::vec3(x, y, 1000.0f), glm::vec2(size, size), glm::vec4(1, 0, 0, rand() % 1000 / 1000.0f))
{
	float angleOffset = Utils::random(-20.0f, 20.0f);

	float vel = Utils::random(20.0f, 100.0f);
	float a = glm::radians(angleDeg + angleOffset);

	m_Dx = std::cosf(a) * vel;
	m_Dy = std::sinf(a) * vel;

	m_Duration = Utils::random(0.2f, 1.0f);
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
