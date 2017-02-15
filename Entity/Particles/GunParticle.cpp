#include "GunParticle.h"

GunParticle::GunParticle(float x, float y, float angle, float movespeed)
	: Particle(x, y, 4, angle)
{
	float maxAngle = 90.0f;
	float angleOffset = Utils::random(-maxAngle, maxAngle);

	glm::vec4 colour = glm::vec4(1, 0.5f + (rand() % 1000 / 2000.0f), 0, rand() % 1000 / 1000.0f);
	setColor(colour);

	float vel = Utils::random(50.0f, 400.0f);// +280.0f;// +movespeed;
	float a = angle + glm::radians(angleOffset);

	float scale = (1.0f - (std::abs(angleOffset) / maxAngle)) + 0.1f;
	float velScale = scale + 0.1f;
	scale = Utils::random(0.0f, 1.0f);
	scaleUp(scale);

	m_Dx = std::cosf(a) * vel * velScale;
	m_Dy = std::sinf(a) * vel * velScale;
	m_Dx += std::cosf(angle) * movespeed;
	m_Dy += std::sinf(angle) * movespeed;

	m_Duration = Utils::random(0.1f, 0.3f);
}
