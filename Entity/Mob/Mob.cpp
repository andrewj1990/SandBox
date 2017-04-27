#include "Mob.h"

Mob::Mob(const glm::vec3& position, const glm::vec2& size, Texture* texture, std::unique_ptr<Player>& player)
	: Entity(position, size, texture), m_Player(player), m_LifeBar(size.x)
{
	m_ActionIndex = -1;
	m_AttackRange = 0;
	m_MaxHP = 0;
	m_HP = 0;
}

void Mob::damage(float amount)
{
	if (m_HP > 0) m_HP -= amount;
	else m_HP = 0;
}

void Mob::update(float timeElapsed)
{
	m_LifeBar.update(m_HP / m_MaxHP, timeElapsed);
}

void Mob::playAction(float timeElapsed)
{
	// use random action
	if (m_ActionIndex == -1 && m_Actions.size() > 0)
	{
		// 0 reserved for damageAction
		m_ActionIndex = Utils::random(1, m_Actions.size());
		m_Actions[m_ActionIndex]->init();
	}
	else
	{
		if (!m_Actions[m_ActionIndex]->isComplete())
		{
			m_Actions[m_ActionIndex]->play(*this, *m_Player, timeElapsed);
		}
		else
		{
			m_ActionIndex = -1;
		}

	}
}
