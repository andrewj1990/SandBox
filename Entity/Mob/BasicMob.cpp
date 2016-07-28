#include "BasicMob.h"
#include "..\..\Level\Terrain.h"

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
	//std::cout << "damaged : " << amount << "\n";
	m_Life -= amount;
}

void BasicMob::update(const Terrain& terrain, float timeElapsed)
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

	if (terrain.isSolid(m_Sprite.getPosition().x + dx, m_Sprite.getPosition().y) ||
		terrain.isSolid(m_Sprite.getPosition().x + m_Sprite.getSize().x + dx, m_Sprite.getPosition().y) ||
		terrain.isSolid(m_Sprite.getPosition().x + dx, m_Sprite.getPosition().y + m_Sprite.getSize().y) ||
		terrain.isSolid(m_Sprite.getPosition().x + m_Sprite.getSize().x + dx, m_Sprite.getPosition().y + m_Sprite.getSize().y))
	{
		dx = 0.0f;
	}

	if (terrain.isSolid(m_Sprite.getPosition().x, m_Sprite.getPosition().y + dy) ||
		terrain.isSolid(m_Sprite.getPosition().x, m_Sprite.getPosition().y + m_Sprite.getSize().y + dy) ||
		terrain.isSolid(m_Sprite.getPosition().x + m_Sprite.getSize().x, m_Sprite.getPosition().y + dy) ||
		terrain.isSolid(m_Sprite.getPosition().x + m_Sprite.getSize().x, m_Sprite.getPosition().y + m_Sprite.getSize().y + dy))
	{
		dy = 0.0f;
	}

	m_Sprite.addDirection(dx, dy);
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

	//if (terrain.isSolid(m_Position.x + dx, m_Position.y) ||
	//	terrain.isSolid(m_Position.x + m_Size.x + dx, m_Position.y) ||
	//	terrain.isSolid(m_Position.x + dx, m_Position.y + m_Size.y) ||
	//	terrain.isSolid(m_Position.x + m_Size.x + dx, m_Position.y + m_Size.y))
	//{
	//	dx = 0.0f;
	//}

	//if (terrain.isSolid(m_Position.x, m_Position.y + dy) ||
	//	terrain.isSolid(m_Position.x, m_Position.y + m_Size.y + dy) ||
	//	terrain.isSolid(m_Position.x + m_Size.x, m_Position.y + dy) ||
	//	terrain.isSolid(m_Position.x + m_Size.x, m_Position.y + m_Size.y + dy))
	//{
	//	dy = 0.0f;
	//}

	m_Sprite.addDirection(dx, dy);
}

void BasicMob::render(Renderer& renderer)
{
	renderer.render(*this);
}
