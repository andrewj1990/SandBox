#include "MoveAction.h"
#include "..\..\Entity.h"

MoveAction::MoveAction()
	: Action()
{
}

void MoveAction::init()
{
	m_CumulativeTime = 0.0f;
	m_Duration = 1.0f;
	m_Complete = false;
}

void MoveAction::play(Entity& mob, Entity& player, float timeElapsed)
{
	m_CumulativeTime += timeElapsed;

	if (m_CumulativeTime > m_Duration)
	{
		m_Complete = true;
		return;
	}

	float angle = mob.getAngle(player);

	//float angle = std::atan2f(dy, dx);

	// if collides with player then set action as complete
	if (mob.collide(player))
	{
		m_Complete = true;
	}
	else
	{
		mob.getSprite().addDirection(std::cosf(angle), std::sinf(angle));
	}

}
