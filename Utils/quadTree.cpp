#include "quadTree.h"
#include "..\Entity\Entity.h"

QuadTree::QuadTree(int level, BoundingBox bounds)
	: m_Level(level), m_Bounds(bounds), ne(nullptr), se(nullptr), sw(nullptr), nw(nullptr)
{
	max_objects = 20;
	max_levels = 5;

}

QuadTree::~QuadTree()
{
}

void QuadTree::split()
{
	int subWidth = m_Bounds.width / 2;
	int subHeight = m_Bounds.height / 2;
	int x = m_Bounds.x;
	int y = m_Bounds.y;

	nw = std::unique_ptr<QuadTree>(new QuadTree(m_Level + 1, BoundingBox(x, y, subWidth, subHeight)));
	ne = std::unique_ptr<QuadTree>(new QuadTree(m_Level + 1, BoundingBox(x + subWidth, y, subWidth, subHeight)));
	sw = std::unique_ptr<QuadTree>(new QuadTree(m_Level + 1, BoundingBox(x, y + subHeight, subWidth, subHeight)));
	se = std::unique_ptr<QuadTree>(new QuadTree(m_Level + 1, BoundingBox(x + subWidth, y + subHeight, subWidth, subHeight)));
}

int QuadTree::getIndex(Renderable* data)
{
	int index = -1;
	double vmid = m_Bounds.x + (m_Bounds.width / 2);
	double hmid = m_Bounds.y + (m_Bounds.height / 2);

	bool topQuad = data->getPosition().y < hmid && data->getPosition().y + data->getSize().y < hmid;
	bool botQuad = data->getPosition().y > hmid;

	if (data->getPosition().x < vmid && data->getPosition().x + data->getSize().x < vmid)
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
	else if (data->getPosition().x > vmid)
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

int QuadTree::getIndex(Entity * data)
{
	int index = -1;
	double vmid = m_Bounds.x + (m_Bounds.width / 2);
	double hmid = m_Bounds.y + (m_Bounds.height / 2);

	bool topQuad = data->getSprite().getPosition().y < hmid && data->getSprite().getPosition().y + data->getSprite().getSize().y < hmid;
	bool botQuad = data->getSprite().getPosition().y > hmid;

	if (data->getSprite().getPosition().x < vmid && data->getSprite().getPosition().x + data->getSprite().getSize().x < vmid)
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
	else if (data->getSprite().getPosition().x > vmid)
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

int QuadTree::getIndex(float x, float y, float w, float h)
{
	int index = -1;
	double vmid = m_Bounds.x + (m_Bounds.width / 2);
	double hmid = m_Bounds.y + (m_Bounds.height / 2);

	bool topQuad = y < hmid && y + h < hmid;
	bool botQuad = y > hmid;

	if (x < vmid && x + w < vmid)
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
	else if (x > vmid)
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

void QuadTree::printObjects()
{
	std::cout << "num objects : " << m_Objects.size() << "\n";
}

void QuadTree::insert(Renderable* data)
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

	//if (m_Bounds.contains(*data)) 
	m_Objects.push_back(data);

	if (m_Objects.size() > max_objects && m_Level < max_levels)
	{
		if (nw == nullptr) split();

		for (auto i = m_Objects.cbegin(); i != m_Objects.cend(); )
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

void QuadTree::insert(Entity* data)
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

	//if (m_Bounds.contains(*data)) 
	m_Entities.push_back(data);

	if (m_Objects.size() > max_objects && m_Level < max_levels)
	{
		if (nw == nullptr) split();

		for (auto i = m_Entities.cbegin(); i != m_Entities.cend(); )
		{
			int index = getIndex(*i);
			if (index != -1)
			{
				if (index == 0)	ne->insert(*i);
				else if (index == 1) nw->insert(*i);
				else if (index == 2) sw->insert(*i);
				else if (index == 3) se->insert(*i);
				i = m_Entities.erase(i);
			}
			else
			{
				++i;
			}
		}
	}

}

void QuadTree::retrieve(std::vector<Renderable*>& data, Renderable* range)
{
	auto index = getIndex(range);

	if (index != -1 && nw != nullptr)
	{
		if (index == 0)	ne->retrieve(data, range);
		else if (index == 1) nw->retrieve(data, range);
		else if (index == 2) sw->retrieve(data, range);
		else if (index == 3) se->retrieve(data, range);
	}

	std::copy(m_Objects.begin(), m_Objects.end(), std::back_inserter(data));
}

void QuadTree::retrieve(std::vector<Renderable*>& data, float x, float y, float w, float h)
{
	auto index = getIndex(x, y, w, h);

	if (index != -1 && nw != nullptr)
	{
		if (index == 0)	ne->retrieve(data, x, y, w, h);
		else if (index == 1) nw->retrieve(data, x, y, w, h);
		else if (index == 2) sw->retrieve(data, x, y, w, h);
		else if (index == 3) se->retrieve(data, x, y, w, h);
	}

	std::copy(m_Objects.begin(), m_Objects.end(), std::back_inserter(data));
}

void QuadTree::retrieve(std::vector<Entity*>& data, Renderable* range)
{
	auto index = getIndex(range);

	if (index != -1 && nw != nullptr)
	{
		if (index == 0)	ne->retrieve(data, range);
		else if (index == 1) nw->retrieve(data, range);
		else if (index == 2) sw->retrieve(data, range);
		else if (index == 3) se->retrieve(data, range);
	}

	std::copy(m_Entities.begin(), m_Entities.end(), std::back_inserter(data));
}

void QuadTree::retrieve(std::vector<Entity*>& data, float x, float y, float w, float h)
{
	auto index = getIndex(x, y, w, h);

	if (index != -1 && nw != nullptr)
	{
		if (index == 0)	ne->retrieve(data, x, y, w, h);
		else if (index == 1) nw->retrieve(data, x, y, w, h);
		else if (index == 2) sw->retrieve(data, x, y, w, h);
		else if (index == 3) se->retrieve(data, x, y, w, h);
	}

	std::copy(m_Entities.begin(), m_Entities.end(), std::back_inserter(data));
}


void QuadTree::getBounds(std::vector<BoundingBox>& bounds)
{
	bounds.push_back(m_Bounds);

	if (nw == nullptr) return;

	nw->getBounds(bounds);
	ne->getBounds(bounds);
	sw->getBounds(bounds);
	se->getBounds(bounds);

}