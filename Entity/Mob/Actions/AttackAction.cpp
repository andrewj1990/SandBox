#include "AttackAction.h"
#include "..\..\Entity.h"

AttackAction::AttackAction()
	: Action()
{
}

void AttackAction::init()
{
	m_Complete = false;
	m_Duration = 1.0f;
}

void AttackAction::play(Entity& mob, Entity& player, float timeElapsed)
{	
	float angle = mob.getAngle(player);

	mob.shoot(angle);

	m_Complete = true;
}

AOEAttackAction::AOEAttackAction()
	: Action()
{
}

void AOEAttackAction::init()
{
	m_Complete = false;
	m_Duration = 1.0f;
}

void AOEAttackAction::play(Entity& mob, Entity& player, float timeElapsed)
{
	float angle = glm::degrees(mob.getAngle(player)) - 90.0f;

	for (int i = 0; i < 180; i += 20)
	{
		mob.shoot(glm::radians(angle + (float)i));
	}

	m_Complete = true;
}
