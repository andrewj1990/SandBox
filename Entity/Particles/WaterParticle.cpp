#include "WaterParticle.h"

WaterParticle::WaterParticle(float x, float y, float angleDeg, float vel)
	: Particle(x, y, 4, angleDeg)
{
	float a = glm::radians(angleDeg);

	float colour = Utils::random(0.7f, 1.0f);
	float alpha = Utils::random(0.4f, 1.0f);
	setColor(glm::vec4(colour, colour, colour, alpha));

	float velOffset = Utils::random(0.0f, 20.0f);
	m_Dx = std::cosf(a) * (vel + velOffset);
	m_Dy = std::sinf(a) * (vel + velOffset);

	float scale = Utils::random(1.0f, 1.3f);
	scaleUp(scale);

	m_Duration = Utils::random(1.5f, 2.0f);
}

void WaterParticle::update(Region& region, float timeElapsed)
{
	if (m_Duration <= 0 || region.getTileType(getCenterX(), getCenterY()) != TileType::SHALLOW_WATER)
	{
		m_Destroy = true;
		return;
	}

	m_Dx *= 0.99f;
	m_Dy *= 0.99f;
	fade();

	addDirection(m_Dx * timeElapsed, m_Dy * timeElapsed);
	m_Duration -= timeElapsed;

}
