#pragma once

#include <queue>
#include <unordered_set>
#include <glm\glm.hpp>

#include "..\Utils\Utils.h"

class Region;

struct KeyFuncs
{
	size_t operator()(const glm::vec2& v) const
	{
		return std::hash<float>()(v.x) ^ std::hash<float>()(v.y);
	}

	bool operator()(const glm::vec2& a, const glm::vec2& b) const
	{
		return a.x == b.x && a.y == b.y;
	}
};

class TileRipple
{
public:
	TileRipple(float x, float y);

	bool update(Region& region);

private:
	void pushQueue(float x, float y);
	const glm::vec2& popQueue();

private:
	float m_RippleChance;

	std::queue<glm::vec2> m_Queue;
	std::unordered_set<glm::vec2, KeyFuncs, KeyFuncs> m_Set;

};