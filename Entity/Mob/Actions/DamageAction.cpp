#include "DamageAction.h"

DamageAction::DamageAction()
	: Action()
{

}

void DamageAction::init()
{
	m_CumulativeTime = 0.0f;
	m_Duration = 0.1f;

	m_Complete = false;
}

void DamageAction::play(Mob& mob, Entity& player, float timeElapsed)
{
	mob.setState(State::HIT);

	m_Duration -= timeElapsed;
	if (m_Duration <= 0)
	{
		m_Complete = true;
	}

	if (!m_Complete)
	{
		mob.setColor(glm::vec4(1, 0, 0, 1));

		float angle = mob.getAngle();
		mob.addDirection(std::cosf(angle), std::sinf(angle));
	}
	else
	{
		mob.setColor(glm::vec4(1, 1, 1, 1));
	}

}
