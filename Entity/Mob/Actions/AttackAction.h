#pragma once

#include "Action.h"
#include "..\..\..\Graphics\Window.h"
#include "..\..\..\Utils\timer.h"

class AttackAction : public Action
{
public:
	AttackAction();

	void init() override;
	void play(Mob& mob, Entity& player, float timeElapsed) override;


protected:
	float m_AttackSpeed;
	float m_AttackFrame;

	Timer m_Timer;
};

class AOEAttackAction : public AttackAction
{
public:
	AOEAttackAction();

	void init() override;
	void play(Mob& mob, Entity& player, float timeElapsed) override;

private:
};

class ChannellingAttack : public AttackAction
{
public:
	ChannellingAttack();

	void init() override;
	void play(Mob& mob, Entity& player, float timeElapsed) override;
private:
	float m_Length;
	float m_Angle;

	Timer m_Timer;

};