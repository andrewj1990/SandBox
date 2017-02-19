#include "BasicMob.h"

BasicMob::BasicMob(float x, float y)
	: Entity(glm::vec3(x, y, 0), glm::vec2(32, 32), TextureManager::get("Textures/Player/PlayerSpritesheet10.png"))
{
	m_Life = 10;
}

BasicMob::~BasicMob()
{
}

void BasicMob::damage(int amount)
{
	m_Life -= amount;
}

void BasicMob::update(float timeElapsed)
{
	if (m_Life <= 0)
	{
		m_Destroy = true;
	}

	m_CumulativeTime += timeElapsed;
	if (m_CumulativeTime > 1.0f)
	{
		m_CumulativeTime = 0;
		m_Dx = Utils::random(-1.0f, 1.0f);
		m_Dy = Utils::random(-1.0f, 1.0f);
	}

	float dx = m_Dx;
	float dy = m_Dy;

	addDirection(dx, dy);
}

void BasicMob::render(Renderer& renderer)
{
	//renderer.render(*this);
}
