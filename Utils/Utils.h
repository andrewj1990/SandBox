#pragma once

#include <cstdlib>
#include <memory>

#include "..\Entity\Mob\Actions\Action.h"
#include "..\Entity\Mob\Actions\MoveAction.h"

enum class ActionType {
	MOVE, ATTACK, END
};

class Utils
{
public:
	static float random();
	static int random(int lowerBound, int upperBound);
	static float random(float lowerBound, float upperBound);

	static std::unique_ptr<Action> getRandomAction();
};