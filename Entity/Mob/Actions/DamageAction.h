#pragma once

#include "Action.h"
#include "..\Mob.h"

class DamageAction : public Action
{
public:
	DamageAction();

	void init() override;
	void play(Mob& mob, Entity& player, float timeElapsed) override;

private:

};