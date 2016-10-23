#pragma once

#include <vector>
#include <iterator>
#include <iostream>
#include "../Graphics/renderable.h"
#include "BoundingBox.h"

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