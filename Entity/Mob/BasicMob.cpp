#include "BasicMob.h"

BasicMob::BasicMob(float x, float y)
	: Entity(glm::vec3(x, y, 0), glm::vec2(64, 64), TextureManager::get("Textures/Mobs/mob.png"))
{
	m_Life = 10;

	m_CollisionBox = std::make_shared<BoundingBox>(x, y, 64, 64);
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
	m_CollisionBox->x = m_Position.x;
	m_CollisionBox->y = m_Position.y;

}

void BasicMob::render(Renderer& renderer)
{
	renderer.render(*this);
}
