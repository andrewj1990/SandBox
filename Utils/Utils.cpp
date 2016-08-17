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

bool Utils::quadCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2)
{
	if ((x1 > x2 && x1 < x2 + w2 && y1 > y2 && y1 < y2 + h2) ||
		(x1 + w1 > x2 && x1 + w1 < x2 + w2 && y1 > y2 && y1 < y2 + h2) ||
		(x1 > x2 && x1 < x2 + w2 && y1 + h1 > y2 && y1 + h1 < y2 + h2) ||
		(x1 + w1 > x2 && x1 + w1 < x2 + w2 && y1 + h1 > y2 && y1 + h1 < y2 + h2))
	{
		return true;
	}

	if ((x2 > x1 && x2 < x1 + w1 && y2 > y1 && y2 < y1 + h1) ||
		(x2 + w2 > x1 && x2 + w2 < x1 + w1 && y2 > y1 && y2 < y1 + h1) ||
		(x2 > x1 && x2 < x1 + w1 && y2 + h2 > y1 && y2 + h2 < y1 + h1) ||
		(x2 + w2 > x1 && x2 + w2 < x1 + w1 && y2 + h2 > y1 && y2 + h2 < y1 + h1))
	{
		return true;
	}


	return false;
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
		//return std::unique_ptr<AttackAction>(new AttackAction());
		return std::unique_ptr<AOEAttackAction>(new AOEAttackAction());
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
