#pragma once

#include <glm\glm.hpp>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include "quadTree.h"
#include <iostream>
#include "Node.h"
#include <functional>
#include <cstdlib>
#include <algorithm>

class AStar
{
public:
	static int heuristic(const Node& a, const Node& b);
	static int cost(Node& node, const std::unique_ptr<QuadTree>& qt);

	static std::unordered_map<int, Node> bfsSearch(int x, int y, const glm::vec2& end);

	static std::unordered_map<int, Node> aStarSearch(int x, int y, const glm::vec2& end, const std::unique_ptr<QuadTree>& qt);
	static glm::vec2 aStarSearchDirection(const Entity& start, const Entity& end, const std::unique_ptr<QuadTree>& qt);

private:
	AStar() {}

};