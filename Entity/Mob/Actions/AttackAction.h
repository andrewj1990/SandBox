#pragma once

#include "Action.h"
#include "..\..\..\Graphics\Window.h"

class AttackAction : public Action
{
public:
	AttackAction();

	void init() override;
	void play(Entity& mob, Entity& player, float timeElapsed) override;


private:

};

class AOEAttackAction : public Action
{
public:
	AOEAttackAction();

	void init() override;
	void play(Entity& mob, Entity& player, float timeElapsed) override;

};
