#pragma once

#include "Action.h"
#include "..\..\..\Graphics\Window.h"

class MoveAction : public Action
{
public:
	MoveAction();

	void play(Entity& entity) override;

private:

};