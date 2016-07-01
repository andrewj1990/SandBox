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

class AStar
{
public:
	static int heuristic(const Node& a, const Node& b);
	static int cost(Node& node, const std::unique_ptr<QuadTree>& qt);

	static std::unordered_map<int, Node> bfsSearch(int x, int y, glm::vec2 end);

	static std::unordered_map<int, Node> aStarSearch(int x, int y, glm::vec2 end, const std::unique_ptr<QuadTree>& qt);

private:
	AStar() {}

};


////class Astar
////{
////public:
////	Astar();
////
////
////private:
////		
////};
//
////inline int heuristic(SquareGrid::Location a, SquareGrid::Location b) {
////	int x1, y1, x2, y2;
////	tie(x1, y1) = a;
////	tie(x2, y2) = b;
////	return abs(x1 - x2) + abs(y1 - y2);
////}
////
////template<typename Graph>
////void a_star_search
////(const Graph& graph,
////	typename Graph::Location start,
////	typename Graph::Location goal,
////	unordered_map<typename Graph::Location, typename Graph::Location>& came_from,
////	unordered_map<typename Graph::Location, int>& cost_so_far)
////{
////	typedef typename Graph::Location Location;
////	PriorityQueue<Location> frontier;
////	frontier.put(start, 0);
////
////	came_from[start] = start;
////	cost_so_far[start] = 0;
////
////	while (!frontier.empty()) {
////		auto current = frontier.get();
////
////		if (current == goal) {
////			break;
////		}
////
////		for (auto next : graph.neighbors(current)) {
////			int new_cost = cost_so_far[current] + graph.cost(current, next);
////			if (!cost_so_far.count(next) || new_cost < cost_so_far[next]) {
////				cost_so_far[next] = new_cost;
////				int priority = new_cost + heuristic(next, goal);
////				frontier.put(next, priority);
////				came_from[next] = current;
////			}
////		}
////	}
////}