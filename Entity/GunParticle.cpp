#include "GunParticle.h"

GunParticle::GunParticle(float x, float y, float angle)
	: Particle(x, y)
{
	float angleOffset = 5.0f - (rand() % 1000 / 100.0f);

	glm::vec4 colour = glm::vec4(1, 0.5f + (rand() % 1000 / 2000.0f), 0, rand() % 1000 / 1000.0f);
	m_Colour = colour;

	float velX = 100.0f + (rand() % 1000 / 10.0f);
	float velY = 100.0f + (rand() % 1000 / 10.0f);
	float a = angle + glm::radians(angleOffset);

	m_Dx = std::cosf(a) * velX;
	m_Dy = std::sinf(a) * velY;

	m_Duration = 0.1f + (rand() % 1000) / 4000.0f;
}
