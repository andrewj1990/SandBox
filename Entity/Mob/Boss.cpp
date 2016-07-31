#include "Boss.h"

Boss::Boss(Player& player, float x, float y)
	: Entity(glm::vec3(x, y, 0), glm::vec2(32, 32), TextureManager::get("Textures/Player/PlayerSpritesheet10.png")), m_Player(player)
{
	m_ActionIndex = -1;

	m_Actions = Utils::getRandomActionList(3);
}

void Boss::damage(int amount)
{
}

void Boss::update(const Terrain& terrain, float timeElapsed)
{
	// use random action
	if (m_ActionIndex == -1 && m_Actions.size() > 0)
	{
		m_ActionIndex = Utils::random(0, m_Actions.size());
		m_Actions[m_ActionIndex]->init();
	}
	else
	{
		if (!m_Actions[m_ActionIndex]->isComplete())
		{
			m_Actions[m_ActionIndex]->play(*this, m_Player, timeElapsed);
		}
		else
		{
			m_ActionIndex = -1;
		}
		
	}

	for (auto it = m_Bullets.begin(); it != m_Bullets.end(); )
	{
		if ((*it)->shouldDestroy())
		{
			it = m_Bullets.erase(it);
		}
		else
		{
			(*it)->update(timeElapsed);
			++it;
		}
	}


}

void Boss::update(float timeElapsed)
{
}

void Boss::render(Renderer& renderer)
{
	renderer.render(m_Bullets);

	renderer.render(m_Sprite);
}

void Boss::shoot(float angle)
{
	m_Bullets.push_back(std::unique_ptr<Bullet>(new Bullet(getCenterX(), getCenterY(), angle)));
}
