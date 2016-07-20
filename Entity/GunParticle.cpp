#include "GunParticle.h"

GunParticle::GunParticle(float x, float y, float angle)
	: Particle(x, y, angle)
{
	float angleOffset = Utils::random(-10.0f, 10.0f);

	glm::vec4 colour = glm::vec4(1, 0.5f + (rand() % 1000 / 2000.0f), 0, rand() % 1000 / 1000.0f);
	m_Colour = colour;

	float vel = Utils::random(100.0f, 200.0f);
	float a = angle + glm::radians(angleOffset);

	m_Dx = std::cosf(a) * vel;
	m_Dy = std::sinf(a) * vel;

	m_Duration = Utils::random(0.1f, 0.3f);
}
