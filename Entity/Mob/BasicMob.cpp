#include "BasicMob.h"

BasicMob::BasicMob(float x, float y, std::unique_ptr<Player>& player)
	: Mob(glm::vec3(x, y, 0), glm::vec2(64, 64), TextureManager::get("Textures/Mobs/mob3.png"), player)
{
	m_Life = 10;

	float sizeFactorX = getWidth() / 32.0f;
	float sizeFactorY = getHeight() / 32.0f;
	m_CollisionBox = std::make_shared<BoundingBox>(x + (sizeFactorX * 10), y, (sizeFactorX * 10), sizeFactorY + 32);

	m_Actions.push_back(std::make_unique<MoveAction>());
	//m_Actions.push_back(std::make_unique<AttackAction>());
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

	//m_CumulativeTime += timeElapsed;
	//if (m_CumulativeTime > 1.0f)
	//{
	//	m_CumulativeTime = 0;
	//	m_Dx = Utils::random(-1.0f, 1.0f);
	//	m_Dy = Utils::random(-1.0f, 1.0f);
	//}

	//float dx = m_Dx;
	//float dy = m_Dy;

	//addDirection(dx, dy);

	playAction(timeElapsed);

	float sizeFactorX = getWidth() / 32.0f;
	float sizeFactorY = getHeight() / 32.0f;
	m_CollisionBox->x = m_Position.x + (sizeFactorX * 10);
	m_CollisionBox->y = m_Position.y;

}

void BasicMob::render(Renderer& renderer)
{
	renderer.render(*this);
}
