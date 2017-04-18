#pragma once

#include "Action.h"
#include "..\..\..\Graphics\Window.h"

class MoveAction : public Action
{
public:
	MoveAction();

	void init() override;
	void play(Mob& mob, Entity& player, float timeElapsed) override;


private:
	float m_CumulativeTime;
	float m_Angle;

	float m_FollowDistanceSqrd;
};