#include "astar.h"
#include "..\Entity\Entity.h"

int AStar::heuristic(const Node& a, const Node& b)
{
	float dx = abs((float)a.x - (float)b.x);
	float dy = abs((float)a.y - (float)b.y);

	float D = 1.0f;
	float D2 = 1.0f;

	//return abs(x1 - x2) + abs(y1 - y2);
	return (int)(D * (dx + dy) + (D2 - 2 * D) * std::min(dx, dy));
}

int AStar::cost(Node& node, const std::unique_ptr<QuadTree>& qt)
{
	std::vector<Renderable*> platforms;
	qt->retrieve(platforms, node.x, node.y, 0, 0);
	for (Renderable* platform : platforms)
	{
		const int& px = platform->getPosition().x;
		const int& py = platform->getPosition().y;
		const int& w = platform->getSize().x;
		const int& h = platform->getSize().y;

		if (node.x <= px + w && node.x >= px && node.y >= py && node.y<= py + h)
		{
			return 400;
		}
	}

	// if the current node is next to a platform then return a reduced cost
	for (const Node& neighbors : node.neighbors())
	{
		platforms.clear();
		qt->retrieve(platforms, neighbors.x, neighbors.y, 0, 0);
		for (Renderable* platform : platforms)
		{
			const int& px = platform->getPosition().x;
			const int& py = platform->getPosition().y;
			const int& w = platform->getSize().x;
			const int& h = platform->getSize().y;

			if (neighbors.x <= px + w && neighbors.x >= px && neighbors.y >= py && neighbors.y <= py + h)
			{
				return 1;
			}
		}


	}

	return 5;
	//return (rand() % 10);
}

std::unordered_map<int, Node> AStar::bfsSearch(int x, int y, const glm::vec2& end)
{
	Node node(x, y);
	std::queue<Node> frontier;
	frontier.push(node);

	Node endNode(end.x, end.y);
	
	std::unordered_map<int, Node> came_from;

	came_from[node.getInt()] = node;

	while (!frontier.empty())
	{
		Node current = frontier.front();
		frontier.pop();

		if (current.getInt() == endNode.getInt()) return came_from;
		// if already visited then continue
		for (Node next : current.neighbors())
		{
			// if already visited then continue
			if (!came_from.count(next.getInt()))
			{
				frontier.push(next);
				came_from[next.getInt()] = current;
			}

		}
	}

	return came_from;

}

std::unordered_map<int, Node> AStar::aStarSearch(int x, int y, const glm::vec2& end, const std::unique_ptr<QuadTree>& qt)
{
	Node node(x, y);
	typedef std::pair<int, Node> PQElement;
	//std::priority_queue<Node> frontier;
	std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> frontier;
	//frontier.push(node);
	frontier.emplace(0, node);
	Node goal(end.x, end.y);

	std::unordered_map<int, Node> came_from;
	std::unordered_map<int, int> cost_so_far;

	came_from[node.getInt()] = node;
	cost_so_far[node.getInt()] = 0;

	while (!frontier.empty())
	{
		Node current = frontier.top().second;
		frontier.pop();

		if (current.getInt() == goal.getInt()) return came_from;
		// if already visited then continue
		for (Node next : current.neighbors())
		{
			int new_cost = cost_so_far[current.getInt()] + cost(next, qt);
			// if already visited then continue
			if (!came_from.count(next.getInt()) || new_cost < cost_so_far[next.getInt()])
			{
				cost_so_far[next.getInt()] = new_cost;
				int priority = new_cost + heuristic(next, goal);
				frontier.emplace(priority, next);
				came_from[next.getInt()] = current;
			}

		}
	}

	return came_from;

}

glm::vec2 AStar::aStarSearchDirection(const Entity& start, const Entity& end, const std::unique_ptr<QuadTree>& qt)
{
//	std::cout << (int)start.getCenterX() / 16 * 16 << ", " << (int)start.getCenterY() / 16 * 16 << " | " << end.getCenterX() / 16 * 16 << ", " << end.getCenterY() / 16 * 16 << "\n";
	
	std::unordered_map<int, Node> path = aStarSearch((int)start.getCenterX() / 16 * 16, (int)start.getCenterY() / 16 * 16, glm::vec2((int)end.getCenterX() / 16 * 16, (int)end.getCenterY() / 16 * 16), qt);

	Node c((int)end.getCenterX() / 16 * 16, (int)end.getCenterY() / 16 * 16);
	Node s((int)start.getCenterX() / 16 * 16, (int)start.getCenterY() / 16 * 16);

	std::vector<Node> p;
	p.push_back(c);
	while (c.getInt() != s.getInt())
	{
		c = path[c.getInt()];
		p.push_back(c);
	}

	float dx = 0.0f;
	float dy = 0.0f;

	if (p.size() > 1)
	{
		p.pop_back();
		Node first = p.back();

		dx = first.x - s.x;
		dy = first.y - s.y;
	}

	return glm::vec2(dx, dy);
}
