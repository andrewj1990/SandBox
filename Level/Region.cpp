#include "Region.h"

Region::Region()
{
	m_X = Window::Instance().getCamera().getPosition().x;
	m_Y = Window::Instance().getCamera().getPosition().y;

	m_SubRegionWidth = 256;
	m_SubRegionHeight = 256;
	m_RegionWidth = Window::Instance().getWidth() + m_SubRegionWidth;
	m_RegionHeight = Window::Instance().getHeight() + m_SubRegionHeight * 2;

	m_CountX = m_RegionWidth / m_SubRegionWidth;
	m_CountY = m_RegionHeight / m_SubRegionHeight;

	for (int x = m_X; x < m_CountX; x++)
	{
		for (int y = m_Y; y < m_CountY; y++)
		{
			load(x, y);
		}
	}
}

void Region::load(int x, int y)
{
	m_Regions.push_back(std::unique_ptr<TileRegion>(new TileRegion(x, y)));
	m_Regions.back()->init(m_Tiles);
}

void Region::unload(int x, int y)
{
	for (auto it = m_Regions.begin(); it != m_Regions.end(); )
	{
		if ((*it)->indexX() == x && (*it)->indexY() == y)
		{
			m_Regions.erase(it);
			return;
		}
		else
		{
			it++;
		}
	}
}

void Region::addTiles(std::unique_ptr<QTree<Renderable>>& quadTree)
{
	for (auto& tileRegion : m_Regions)
	{
		for (auto& tile : tileRegion->getTiles())
		{
			quadTree->insert(tile.get());
		}
	}
}

void Region::removeTiles(float x, float y)
{
	int ix = (int)x / 16 * 16;
	int iy = (int)y / 16 * 16;

	std::string positionInRegion = std::to_string(ix) + "_" + std::to_string(iy);
	auto it = m_Tiles.find(positionInRegion);
	if (it == m_Tiles.end())
	{
		m_Tiles.insert(positionInRegion);

		// remove the tile from the region
		int rx = std::floor(x / m_SubRegionWidth);
		int ry = std::floor(y / m_SubRegionHeight);

		unload(rx, ry);
		load(rx, ry);
	}
}

void Region::update(float timeElapsed)
{
	int x = std::floor(Window::Instance().getCamera().getPosition().x / m_SubRegionWidth);
	int y = std::floor(Window::Instance().getCamera().getPosition().y / m_SubRegionHeight);


	// update regions when camera moves outside current region
	if (m_X != x)
	{
		int delX = m_X < x ? m_X : m_X + m_CountX - 1;
		int addX = m_X < x ? m_X + m_CountX : x;

		for (int i = m_Y; i < m_Y + m_CountY; i++)
		{
			unload(delX, i);
			load(addX, i);
		}

		m_X = x;
	}
	if (m_Y != y)
	{
		int delY = m_Y < y ? m_Y : m_Y + m_CountY - 1;
		int addY = m_Y < y ? m_Y + m_CountY : y;

		for (int i = m_X; i < m_X + m_CountX; i++)
		{
			unload(i, delY);
			load(i, addY);
		}

		m_Y = y;
	}

	float camX = Window::Instance().getCamera().Position.x;
	float camY = Window::Instance().getCamera().Position.y;
	// remove the tile
	if (Window::Instance().isButtonPressed(GLFW_MOUSE_BUTTON_2))
	{
		float mx = camX + Window::Instance().mouseX();
		float my = camY + (Window::Instance().getHeight() - Window::Instance().mouseY());

		removeTiles(mx, my);

	}
}

void Region::render(Renderer& renderer)
{
	for (auto& tileRegion : m_Regions)
	{
		tileRegion->render(renderer);
	}
}
