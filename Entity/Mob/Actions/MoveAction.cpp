#include "MoveAction.h"
#include "..\Mob.h"

MoveAction::MoveAction()
	: Action()
{
}

void MoveAction::init()
{
	m_CumulativeTime = 0.0f;
	m_Duration = 1.0f;
	m_Complete = false;

	m_Angle = glm::radians(Utils::random(0.0f, 360.0f));
	
	float followDistance = 400.0f;
	m_FollowDistanceSqrd = followDistance * followDistance;
}

void MoveAction::play(Mob& mob, Entity& player, float timeElapsed)
{
	m_CumulativeTime += timeElapsed;

	if (m_CumulativeTime > m_Duration)
	{
		m_Complete = true;
		return;
	}

	//float angle = mob.getAngle(player);
	float distSqrd = mob.calcDistanceSqrd(player);
	if (distSqrd < m_FollowDistanceSqrd)
	{
		float angle = mob.getAngle(player);
		m_Angle = angle;
	}

	// if collides with player then set action as complete
	if (mob.collide(player))
	{
		m_Complete = true;
	}
	else
	{
		mob.addDirection(std::cosf(m_Angle), std::sinf(m_Angle));
	}

}
