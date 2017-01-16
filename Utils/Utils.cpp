#include "Utils.h"

float Utils::random()
{
	float r = rand();
	return r;
}

int Utils::random(int lowerBound, int upperBound)
{
	int range = upperBound - lowerBound;
	float r = rand() % range;
	return lowerBound + r;
}

float Utils::random(float lowerBound, float upperBound)
{
	float r = lowerBound + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (upperBound - lowerBound)));
	return r;
}

int Utils::fastFloor(float v)
{
	return v >= 0 ? (int)v : (int)v - 1;
}

bool Utils::quadCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2)
{
	//if ((x1 > x2 && x1 < x2 + w2 && y1 > y2 && y1 < y2 + h2) ||
	//	(x1 + w1 > x2 && x1 + w1 < x2 + w2 && y1 > y2 && y1 < y2 + h2) ||
	//	(x1 > x2 && x1 < x2 + w2 && y1 + h1 > y2 && y1 + h1 < y2 + h2) ||
	//	(x1 + w1 > x2 && x1 + w1 < x2 + w2 && y1 + h1 > y2 && y1 + h1 < y2 + h2))
	//{
	//	return true;
	//}

	//if ((x2 > x1 && x2 < x1 + w1 && y2 > y1 && y2 < y1 + h1) ||
	//	(x2 + w2 > x1 && x2 + w2 < x1 + w1 && y2 > y1 && y2 < y1 + h1) ||
	//	(x2 > x1 && x2 < x1 + w1 && y2 + h2 > y1 && y2 + h2 < y1 + h1) ||
	//	(x2 + w2 > x1 && x2 + w2 < x1 + w1 && y2 + h2 > y1 && y2 + h2 < y1 + h1))
	//{
	//	return true;
	//}

	//return false;

	return (x1 <= x2 + w2 && x1 + w1 >= x2 && y1 <= y2 + h2 && y1 + h1 >= y2);
}

// credit goes to blackpawn : http://blackpawn.com/texts/pointinpoly/default.html
bool Utils::trianglePointCollision(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& p)
{
	glm::vec2 v0 = c - a;
	glm::vec2 v1 = b - a;
	glm::vec2 v2 = p - a;

	float dot00 = glm::dot(v0, v0);
	float dot01 = glm::dot(v0, v1);
	float dot02 = glm::dot(v0, v2);
	float dot11 = glm::dot(v1, v1);
	float dot12 = glm::dot(v1, v2);

	float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	return (u >= 0) && (v >= 0) && (u + v < 1);
}

std::unique_ptr<Action> Utils::getRandomAction()
{
	ActionType type = static_cast<ActionType>(rand() % static_cast<int>(ActionType::END));
	std::cout << "type : " << (int)type << "\n";

	return getAction(type);
}

std::unique_ptr<Action> Utils::getAction(ActionType actionType)
{
	switch (actionType)
	{
	case ActionType::MOVE:
		return std::unique_ptr<MoveAction>(new MoveAction());
	case ActionType::ATTACK:
		return std::unique_ptr<AttackAction>(new AttackAction());
	case ActionType::AOEATTACK:
		return std::unique_ptr<AttackAction>(new AttackAction());
		//return std::unique_ptr<AOEAttackAction>(new AOEAttackAction());
	default:
		return std::unique_ptr<MoveAction>(new MoveAction());
	}
}

std::vector<std::unique_ptr<Action>> Utils::getRandomActionList(int size)
{
	std::vector<int> v(static_cast<int>(ActionType::END));
	std::iota(v.begin(), v.end(), 0);

	std::random_shuffle(v.begin(), v.end());

	std::vector<std::unique_ptr<Action>> actionVec;

	if (size > v.size()) size = v.size();
	for (int i = 0; i < size; i++)
	{
		actionVec.push_back(getAction(static_cast<ActionType>(v[i])));
	}

	return actionVec;
}
