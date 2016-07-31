#include "AttackAction.h"
#include "..\..\Entity.h"

AttackAction::AttackAction()
	: Action()
{
}

void AttackAction::init()
{
	m_Complete = false;
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
}

void AOEAttackAction::play(Entity& mob, Entity& player, float timeElapsed)
{
	float angle = mob.getAngle(player);

	for (int i = 0; i < 360; i += 20)
	{
		mob.shoot(angle + glm::radians((float)i));
	}

	m_Complete = true;
}
