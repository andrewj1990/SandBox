#include "Mob.h"

Mob::Mob(const glm::vec3& position, const glm::vec2& size, Texture* texture, std::unique_ptr<Player>& player)
	: Entity(position, size, texture), m_Player(player)
{
	m_ActionIndex = -1;
}

void Mob::playAction(float timeElapsed)
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
			m_Actions[m_ActionIndex]->play(*this, *m_Player, timeElapsed);
		}
		else
		{
			m_ActionIndex = -1;
		}

	}
}
