#pragma once

#include <cstdlib>
#include <memory>
#include <numeric>
#include <algorithm>

#include "..\Entity\Mob\Actions\Action.h"
#include "..\Entity\Mob\Actions\MoveAction.h"
#include "..\Entity\Mob\Actions\AttackAction.h"
#include "BoundingBox.h"

enum class ActionType {
	MOVE, ATTACK, AOEATTACK, END
};

class Utils
{
public:
	static float random();
	static int random(int lowerBound, int upperBound);
	static float random(float lowerBound, float upperBound);
	static bool chance(int percent);
	static int fastFloor(float v);
	static float lerp(float a, float b, float t);
	static float calcAngleDeg(float fromX, float fromY, float toX, float toY);
	static float calcAngleRad(float fromX, float fromY, float toX, float toY);
	static bool inRange(float px, float py, float tx, float ty, float radius);

	static bool lineIntersection(const glm::vec4& line1, const glm::vec4& line2);
	static glm::vec2 intersection(const BoundingBox& bbox, const glm::vec4& line);
	static void intersect(float sx, float sy, float ex, float ey, const glm::vec4& line, float dirX, float dirY, int& T1, float& endPointX, float& endPointY);
	static glm::mat4 calcTransformMat(float x, float y, float w, float h, float angle);

	static bool quadCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);
	static bool trianglePointCollision(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& p);

	static std::unique_ptr<Action> getRandomAction();
	static std::unique_ptr<Action> getAction(ActionType actionType);
	static std::vector<std::unique_ptr<Action>> getRandomActionList(int size);

};