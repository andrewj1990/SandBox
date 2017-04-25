#include "AttackAction.h"
#include "..\Mob.h"

AttackAction::AttackAction()
	: Action(), m_Timer()
{
}

void AttackAction::init()
{
	m_Complete = false;
	m_Duration = 0.5f;
	m_AttackSpeed = 0.0f;
	m_AttackFrame = 0.0f;
	m_CumulativeTime = 0.0f;
}

void AttackAction::play(Mob& mob, Entity& player, float timeElapsed)
{	
	mob.setState(State::ATTACKING);
	if (m_Timer.elapsed() > 0.3f)
	{
		m_Timer.reset();
		mob.attack(mob.getCenterX(), mob.getCenterY());
	}

	m_Duration -= timeElapsed;
	if (m_Duration < 0)
	{
		m_Complete = true;
	}
}

MeleeAction::MeleeAction()
	: Action(), m_Timer()
{
}

void MeleeAction::init()
{
	m_Complete = false;
	m_Duration = 0.5f;
	m_AttackSpeed = 0.0f;
	m_AttackFrame = 0.0f;
	m_CumulativeTime = 0.0f;
}

void MeleeAction::play(Mob& mob, Entity& player, float timeElapsed)
{
	if (!Utils::inRange(mob.getCenterX(), mob.getCenterY(), player.getCenterX(), player.getCenterY(), 100) && mob.getState() != State::ATTACKING)
	{
		m_Complete = true;
		return;
	}

	mob.setState(State::ATTACKING);
	if (m_Timer.elapsed() > 0.3f)
	{
		m_Timer.reset();
		mob.attack(mob.getCenterX(), mob.getCenterY());
	}

	m_Duration -= timeElapsed;
	if (m_Duration < 0)
	{
		m_Complete = true;
	}
}

AOEAttackAction::AOEAttackAction()
	: AttackAction()
{
}

void AOEAttackAction::init()
{
	m_Complete = false;
	m_Duration = 0.5f;
	m_AttackSpeed = 0.0f;
	m_AttackFrame = 0.0f;
	m_CumulativeTime = 0.0f;
}

void AOEAttackAction::play(Mob& mob, Entity& player, float timeElapsed)
{
	float angle = glm::degrees(mob.getAngle(player)) - 90.0f;

	for (int i = 0; i < 180; i += 20)
	{
		mob.shoot(glm::radians(angle + (float)i));
	}

	m_Duration -= timeElapsed;
	if (m_Duration < 0)
	{
		m_Complete = true;
	}
}

ChannellingAttack::ChannellingAttack()
	: AttackAction(), m_Timer()
{
}

void ChannellingAttack::init()
{
	m_Complete = false;
	m_Duration = 2.0f;
	m_AttackSpeed = 0.0f;
	m_AttackFrame = 0.0f;
	m_CumulativeTime = 0.0f;
	m_Length = 0.0f;
	m_Angle = 0.0f;

	m_Timer.reset();
}

void ChannellingAttack::play(Mob& mob, Entity& player, float timeElapsed)
{
	if (m_Timer.elapsed() > 0.3f)
	{
		m_Timer.reset();

		if (m_Length == 0)
		{
			m_Angle = mob.getAngle(player);
		}

		m_Length += 200.0f;
		float dx = m_Length * std::cosf(m_Angle);
		float dy = m_Length * std::sinf(m_Angle);
		mob.attack(mob.getCenterX() + dx , mob.getCenterY() + dy);
	}

	m_Duration -= timeElapsed;
	if (m_Duration < 0)
	{
		m_Complete = true;
	}
}
