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

	bool intersects(const BoundingBox& box) 
	{
		return (x <= box.x + box.width && x + width >= box.x && y <= box.y + box.height && y + height >= box.y);
	}
};

class QuadTree
{
public:
	QuadTree(int level, BoundingBox bounds);
	~QuadTree();

	void split();

	void retrieve(std::vector<Renderable*>& data, Renderable* range);
	void retrieve(std::vector<Renderable*>& data, float x, float y, float w, float h);
	void retrieve(std::vector<Entity*>& data, Renderable* range);
	void retrieve(std::vector<Entity*>& data, float x, float y, float w, float h);
	void queryRange(std::vector<Renderable*>& data, const BoundingBox& bbox);

	void insert(Renderable* data);
	void insert(Entity* data);
	int getIndex(Renderable* data);
	int getIndex(Entity* data);
	int getIndex(float x, float y, float w, float h);

	void printObjects();

	void getBounds(std::vector<BoundingBox>& bounds);

private:
	int max_objects;
	int max_levels;

	int m_Level;
	std::vector<Renderable*> m_Objects;
	std::vector<Entity*> m_Entities;
	
	BoundingBox m_Bounds;
	std::unique_ptr<QuadTree> ne;
	std::unique_ptr<QuadTree> se;
	std::unique_ptr<QuadTree> sw;
	std::unique_ptr<QuadTree> nw;

};