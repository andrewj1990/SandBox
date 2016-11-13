#pragma once

// referenced https://gamedevelopment.tutsplus.com/tutorials/quick-tip-use-quadtrees-to-detect-likely-collisions-in-2d-space--gamedev-374

#include <vector>
#include <iterator>
#include "BoundingBox.h"

template <class T>
class QTree
{
public:
	QTree<T>(int level, const BoundingBox& bounds);

	void split();


	int getIndex(const std::shared_ptr<T>& data);
	int getIndex(float x, float y, float w, float h);
	void insert(std::shared_ptr<T> data);

	void retrieve(std::vector<std::shared_ptr<T>>& data, const BoundingBox& bbox);

private:
	int max_objects;
	int max_levels;

	int m_Level;
	std::vector<std::shared_ptr<T>> m_Objects;

	BoundingBox m_Bounds;
	std::unique_ptr<QTree<T>> ne;
	std::unique_ptr<QTree<T>> se;
	std::unique_ptr<QTree<T>> sw;
	std::unique_ptr<QTree<T>> nw;

};

template<class T>
inline QTree<T>::QTree(int level, const BoundingBox& bounds)
	: m_Level(level), m_Bounds(bounds), ne(nullptr), se(nullptr), sw(nullptr), nw(nullptr)
{
	max_objects = 40;
	max_levels = 8;
}

template<class T>
inline void QTree<T>::split()
{
	int subWidth = m_Bounds.width / 2;
	int subHeight = m_Bounds.height / 2;
	int x = m_Bounds.x;
	int y = m_Bounds.y;

	nw = std::unique_ptr<QTree<T>>(new QTree<T>(m_Level + 1, BoundingBox(x, y, subWidth, subHeight)));
	ne = std::unique_ptr<QTree<T>>(new QTree<T>(m_Level + 1, BoundingBox(x + subWidth, y, subWidth, subHeight)));
	sw = std::unique_ptr<QTree<T>>(new QTree<T>(m_Level + 1, BoundingBox(x, y + subHeight, subWidth, subHeight)));
	se = std::unique_ptr<QTree<T>>(new QTree<T>(m_Level + 1, BoundingBox(x + subWidth, y + subHeight, subWidth, subHeight)));
}

template<class T>
inline int QTree<T>::getIndex(const std::shared_ptr<T>& data)
{
	int index = -1;
	double vmid = m_Bounds.x + (m_Bounds.width / 2);
	double hmid = m_Bounds.y + (m_Bounds.height / 2);

	bool topQuad = data->getPosition().y < hmid;// && data->getPosition().y + data->getSize().y < hmid;
	bool botQuad = data->getPosition().y >= hmid;

	if (data->getPosition().x < vmid)// && data->getPosition().x + data->getSize().x < vmid)
	{
		if (topQuad)
		{
			index = 1;
		}
		else if (botQuad)
		{
			index = 2;
		}
	}
	else if (data->getPosition().x >= vmid)
	{
		if (topQuad)
		{
			index = 0;
		}
		else if (botQuad)
		{
			index = 3;
		}

	}

	//if (index == -1) std::cout << "uh oh\n";

	return index;
}

template<class T>
inline int QTree<T>::getIndex(float x, float y, float w, float h)
{
	int index = -1;
	double vmid = m_Bounds.x + (m_Bounds.width / 2);
	double hmid = m_Bounds.y + (m_Bounds.height / 2);

	bool topQuad = y < hmid;// && y + h < hmid;
	bool botQuad = y >= hmid;

	if (x < vmid)// && x + w < vmid)
	{
		if (topQuad)
		{
			index = 1;
		}
		else if (botQuad)
		{
			index = 2;
		}
	}
	else if (x >= vmid)
	{
		if (topQuad)
		{
			index = 0;
		}
		else if (botQuad)
		{
			index = 3;
		}

	}

	return index;
}

template<class T>
inline void QTree<T>::insert(std::shared_ptr<T> data)
{
	// if there are child nodes then find the child node that we insert into
	if (nw != nullptr)
	{
		int index = getIndex(data);

		if (index != -1)
		{
			if (index == 0)	ne->insert(data);
			else if (index == 1) nw->insert(data);
			else if (index == 2) sw->insert(data);
			else if (index == 3) se->insert(data);
			return;
		}
	}

	m_Objects.push_back(data);

	if (m_Objects.size() > max_objects && m_Level < max_levels)
	{
		if (nw == nullptr) split();

		// remove objects from current bounds and add to leaf nodes
		for (auto i = m_Objects.begin(); i != m_Objects.end(); )
		{
			int index = getIndex(*i);
			if (index != -1)
			{
				if (index == 0)	ne->insert(*i);
				else if (index == 1) nw->insert(*i);
				else if (index == 2) sw->insert(*i);
				else if (index == 3) se->insert(*i);
				i = m_Objects.erase(i);
			}
			else
			{
				++i;
			}
		}
	}

}

template<class T>
inline void QTree<T>::retrieve(std::vector<std::shared_ptr<T>>& data, const BoundingBox& bbox)
{
	if (m_Bounds.intersects(bbox))
	{
		if (ne != nullptr) ne->retrieve(data, bbox);
		if (nw != nullptr) nw->retrieve(data, bbox);
		if (sw != nullptr) sw->retrieve(data, bbox);
		if (se != nullptr) se->retrieve(data, bbox);

		std::copy(m_Objects.begin(), m_Objects.end(), std::back_inserter(data));
	}

}
