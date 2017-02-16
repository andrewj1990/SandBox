#include "TileRipple.h"
#include "..\Region.h"

TileRipple::TileRipple(float x, float y)
{
	pushQueue(x, y + Settings::Instance().TILE_SIZE);
	m_RippleChance = Utils::random(0.0f, 50.0f);
}

bool TileRipple::update(Region& region)
{
	if (m_Queue.empty() || m_Queue.size() > 50) return false;

	const glm::vec2& front = popQueue();

	float rand = Utils::random(0.0f, 70.0f);
	if (rand < m_RippleChance)
	{
		if (!region.emptyTile(front.x, front.y))
		{
			region.removeTiles(front.x, front.y);
			for (int i = -1; i <= 1; i++)
			{
				for (int j = -1; j <= 1; j++)
				{
					if (i == 0 && j == 0) continue;
					pushQueue(front.x + Settings::Instance().TILE_SIZE * i, front.y + Settings::Instance().TILE_SIZE * j);
				}
			}
		}
	}

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

const glm::vec2& TileRipple::popQueue()
{
	const glm::vec2& val = m_Queue.front();
	m_Queue.pop();
	m_Set.erase(val);
	return val;
}
