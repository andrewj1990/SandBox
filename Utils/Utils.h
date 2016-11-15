#pragma once

#include <cstdlib>
#include <memory>
#include <numeric>
#include <algorithm>

#include "..\Entity\Mob\Actions\Action.h"
#include "..\Entity\Mob\Actions\MoveAction.h"
#include "..\Entity\Mob\Actions\AttackAction.h"

enum class ActionType {
	MOVE, ATTACK, AOEATTACK, END
};

class Utils
{
public:
	static float random();
	static int random(int lowerBound, int upperBound);
	static float random(float lowerBound, float upperBound);
	static int fastFloor(float v);

	static bool quadCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);
	static bool trianglePointCollision(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& p);

	static std::unique_ptr<Action> getRandomAction();
	static std::unique_ptr<Action> getAction(ActionType actionType);
	static std::vector<std::unique_ptr<Action>> getRandomActionList(int size);

};