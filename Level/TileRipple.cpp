#include "TileRipple.h"
#include "Region.h"

TileRipple::TileRipple(float x, float y)
{
	pushQueue(x, y + Settings::TILE_SIZE);	
	m_RippleChance = Utils::random(0.0f, 50.0f);
}

bool TileRipple::update(Region& region)
{
	if (m_Queue.empty())
	{
		std::cout << "queue empty!!! DESOTRYING\n";
		return false;
	}

	glm::vec2 front = popQueue();

	float rand = Utils::random(0.0f, 100.0f);
	if (rand < m_RippleChance)
	{
		region.removeTiles(front.x, front.y);
		pushQueue(front.x, front.y + Settings::TILE_SIZE);
		pushQueue(front.x + Settings::TILE_SIZE, front.y + Settings::TILE_SIZE);
		pushQueue(front.x - Settings::TILE_SIZE, front.y + Settings::TILE_SIZE);
	}
	else
	{

	}

	//std::cout << "queue size : " << m_Queue.size() << "\n";
	
	return true;
}

void TileRipple::pushQueue(float x, float y)
{
	glm::vec2 val(x, y);
	if (m_Set.find(val) == m_Set.end())
	{
		m_Set.insert(val);
		m_Queue.push(val);
	}
}

glm::vec2 TileRipple::popQueue()
{
	const glm::vec2& val = m_Queue.front();
	m_Queue.pop();
	m_Set.erase(val);
	return val;
}
