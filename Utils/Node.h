#pragma once

#include <vector>

class Node
{
public:
	int x;
	int y;
	std::vector<Node> edges;

	Node() : x(0), y(0) {}
	Node(int x, int y) : x(x), y(y) 
	{
	}

	const std::vector<Node>& neighbors()
	{
		int size = 16;
		if (edges.empty())
		{
			edges.push_back(Node(x + size, y));
			edges.push_back(Node(x, y + size));
			edges.push_back(Node(x - size, y));
			edges.push_back(Node(x, y - size));

			edges.push_back(Node(x - size, y - size));
			edges.push_back(Node(x + size, y - size));
			edges.push_back(Node(x + size, y + size));
			edges.push_back(Node(x - size, y + size));
		}
		return edges;
	}

	int getInt() const
	{
		return x + y * 1280;
	}

	bool operator<(const Node& rhs) const
	{
		return false;
	}

	bool operator()(const Node& a, const Node& b)
	{
		return true;
	}

};