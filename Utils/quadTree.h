#pragma once

#include <vector>
#include <iterator>
#include <iostream>
#include "../Graphics/renderable.h"

struct BoundingBox
{
	int x;
	int y;
	int width;
	int height;

	BoundingBox(int x, int y, int width, int height)
		: x(x), y(y), width(width), height(height)
	{ }

	bool contains(const Renderable& data)
	{
		bool containsX = (x <= data.getPosition().x && data.getPosition().x <= x + width);
		bool containsY = (y <= data.getPosition().y && data.getPosition().y <= y + height);

		return (containsX && containsY);
	}

	bool intersects(const Renderable& box)
	{
		return (x <= box.getPosition().x + box.getSize().x && x + width >= box.getPosition().x && y <= box.getPosition().y + box.getSize().y && y + height >= box.getPosition().y);
	}
};

class QuadTree
{
public:
	QuadTree(int level, BoundingBox bounds);
	~QuadTree();

	void split();

	void retrieve(std::vector<Renderable*>& list, Renderable* range);
	void retrieve(std::vector<Renderable*>& data, float x, float y, float w, float h);
	void insert(Renderable* data);
	int getIndex(Renderable* data);
	int getIndex(float x, float y, float w, float h);

	void printObjects();

	void getBounds(std::vector<BoundingBox>& bounds);

private:
	int max_objects;
	int max_levels;

	int m_Level;
	std::vector<Renderable*> m_Objects;
	
	BoundingBox m_Bounds;
	std::unique_ptr<QuadTree> ne;
	std::unique_ptr<QuadTree> se;
	std::unique_ptr<QuadTree> sw;
	std::unique_ptr<QuadTree> nw;

};