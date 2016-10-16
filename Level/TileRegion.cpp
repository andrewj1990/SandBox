#include "TileRegion.h"

TileRegion::TileRegion(int x, int y)
	: m_IndexX(x), m_IndexY(y)
{
	m_Size = 16;
	m_X = x * (m_Size * m_Size);
	m_Y = y * (m_Size * m_Size);
}

TileRegion::TileRegion(int x, int y, int size)
	: m_Size(size), m_IndexX(x), m_IndexY(y)
{
	m_X = x * (m_Size * m_Size);
	m_Y = y * (m_Size * m_Size);
}

void TileRegion::init()
{
	int tile_size = 16;
	for (int i = 0; i < m_Size; i++)
	{
		for (int j = 0; j < m_Size; j++)
		{
			m_Tiles.push_back(std::unique_ptr<Tile>(new Tile()));
			auto& t = m_Tiles[j + i * m_Size];
			int di = Utils::random(0, 2);
			if (di != 0) {
				t->init(m_X + j * tile_size, m_Y + i * tile_size, glm::vec4(0, 1, 0, 1), false, false);
			}
			else
			{
				t->init(m_X + j * tile_size, m_Y + i * tile_size, glm::vec4(1, 1, 0, 0), false, false);
			}
		}
	}
}

void TileRegion::render(Renderer& renderer) 
{
	renderer.render(m_Tiles);
}
