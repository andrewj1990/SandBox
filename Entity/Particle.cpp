#include "Particle.h"

Particle::Particle(float x, float y)
	: Entity(glm::vec3(x, y, 0.0f), glm::vec2(5, 5), TextureManager::get("Textures/Player/Particle.png"))
{
	m_Sprite.setColor(glm::vec4(1, 1, 1, Utils::random(0.0f, 1.0f)));
	m_Dx = Utils::random(-1.0f, 1.0f);
	m_Dy = Utils::random(-1.0f, 1.0f);

	float vel = 64.0f;
	m_Dx *= vel;
	m_Dy *= vel;

	m_Duration = Utils::random(0.2f, 1.0f);
}

Particle::Particle(float x, float y, float angle)
	: Entity(glm::vec3(x, y, 0.0f), glm::vec2(5, 5), glm::vec4(1, 0, 0, rand() % 1000 / 1000.0f))
{
	float angleOffset = Utils::random(-20.0f, 20.0f);

	float vel = Utils::random(20.0f, 100.0f);
	float a = angle + glm::radians(angleOffset);

	m_Dx = std::cosf(a) * vel;
	m_Dy = std::sinf(a) * vel;

	m_Duration = Utils::random(0.2f, 1.0f);
}

void Particle::update(float timeElapsed)
{
	m_Sprite.addDirection(m_Dx * timeElapsed, m_Dy * timeElapsed);

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
