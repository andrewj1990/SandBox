#include "Region.h"

Region::Region()
{
	m_X = Window::Instance().getCamera().getPosition().x;
	m_Y = Window::Instance().getCamera().getPosition().y;

	// number of tiles per sub region
	m_SubRegionWidth = Settings::Instance().SUB_REGION_TILE_COUNT * Settings::Instance().TILE_SIZE;
	m_SubRegionHeight = Settings::Instance().SUB_REGION_TILE_COUNT * Settings::Instance().TILE_SIZE;
	m_RegionWidth = Window::Instance().getWidth() + m_SubRegionWidth;
	m_RegionHeight = Window::Instance().getHeight() + m_SubRegionHeight;

	m_CountX = (m_RegionWidth / m_SubRegionWidth);
	m_CountY = (m_RegionHeight / m_SubRegionHeight) + 1;

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
	m_Regions.back()->init(m_Tiles, m_DestroyedObjects);
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

void Region::addTiles(std::unique_ptr<QTree<Sprite>>& quadTree)
{
	for (auto& tileRegion : m_Regions)
	{
		for (auto& tile : tileRegion->getTiles())
		{
			quadTree->insert(tile);
		}
	}
}

void Region::addWaterTiles(std::unique_ptr<QTree<Sprite>>& quadTree)
{
	for (auto& tileRegion : m_Regions)
	{
		for (auto& tile : tileRegion->getTiles())
		{
			if (tile->getType() == TileType::SHALLOW_WATER)
			{
				quadTree->insert(tile);
			}
		}
	}
}

void Region::addObjects(std::unique_ptr<QTree<Sprite>>& quadTree)
{
	for (auto& tileRegion : m_Regions)
	{
		for (auto& object : tileRegion->getObjects())
		{
			quadTree->insert(object);
		}
	}
}

void Region::removeTiles(float x, float y, bool exactCoord, bool ripple)
{
	int ix = (int)x / Settings::Instance().TILE_SIZE * Settings::Instance().TILE_SIZE;
	int iy = (int)y / Settings::Instance().TILE_SIZE * Settings::Instance().TILE_SIZE;

	if (!exactCoord)
	{
		ix = x < 0 ? ix - Settings::Instance().TILE_SIZE : ix;
		iy = y < 0 ? iy - Settings::Instance().TILE_SIZE : iy;
	}

	std::string tileGlobalPosition = std::to_string(ix) + "_" + std::to_string(iy);

	auto it = m_Tiles.find(tileGlobalPosition);
	if (it == m_Tiles.end())
	{
		m_Tiles.insert(tileGlobalPosition);

		auto& region = getTileRegion(ix, iy);
		region->removeTile(ix, iy);

		if (ripple) m_Ripples.push_back(std::unique_ptr<TileRipple>(new TileRipple(ix, iy)));

		// reload the uv for the adjacent tiles
		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				if (i == 0 && j == 0) continue;

				int a = ix + i * Settings::Instance().TILE_SIZE;
				int b = iy + j * Settings::Instance().TILE_SIZE;
				reloadTileUV(a, b);
			}
		}
	}
}

void Region::removeObject(float x, float y, bool exactCoord)
{
	int ix = (int)x / Settings::Instance().TILE_SIZE * Settings::Instance().TILE_SIZE;
	int iy = (int)y / Settings::Instance().TILE_SIZE * Settings::Instance().TILE_SIZE;

	if (!exactCoord)
	{
		ix = x < 0 ? ix - Settings::Instance().TILE_SIZE : ix;
		iy = y < 0 ? iy - Settings::Instance().TILE_SIZE : iy;
	}

	std::string tileGlobalPosition = std::to_string(ix) + "_" + std::to_string(iy);

	auto it = m_DestroyedObjects.find(tileGlobalPosition);
	if (it == m_DestroyedObjects.end())
	{
		m_DestroyedObjects.insert(tileGlobalPosition);
	}
}

TileType Region::getTileType(float x, float y)
{
	int ix = x < 0 ? (int)(x - Settings::Instance().TILE_SIZE) / Settings::Instance().TILE_SIZE * Settings::Instance().TILE_SIZE : (int)x / Settings::Instance().TILE_SIZE * Settings::Instance().TILE_SIZE;
	int iy = y < 0 ? (int)(y - Settings::Instance().TILE_SIZE) / Settings::Instance().TILE_SIZE * Settings::Instance().TILE_SIZE : (int)y / Settings::Instance().TILE_SIZE * Settings::Instance().TILE_SIZE;

	const auto& region = getTileRegion(ix, iy);
	return region->getTileType(ix, iy);
}

bool Region::emptyTile(float x, float y)
{
	auto& region = getTileRegion(x, y);
	return region->calculateTile(x, y, m_Tiles);
}

bool Region::getSurfacePosition(float x, float y)
{
	auto& region = getTileRegion(x, y);
	return region->surfaceTile(x, y);
}

void Region::update(float timeElapsed)
{
	int x = Utils::fastFloor(Window::Instance().getCamera().getPosition().x / m_SubRegionWidth);
	int y = Utils::fastFloor(Window::Instance().getCamera().getPosition().y / m_SubRegionHeight);

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

	// remove the tile
	if (Window::Instance().isKeyPressed(GLFW_KEY_H))
	{
		float mx = Window::Instance().getMouseWorldPosX();
		float my = Window::Instance().getMouseWorldPosY();

		removeTiles(mx, my, false, true);
	}

	// get tile type
	if (Window::Instance().isKeyPressed(GLFW_KEY_T))
	{
		float mx = Window::Instance().getMouseWorldPosX();
		float my = Window::Instance().getMouseWorldPosY();

		getTileType(mx, my);
	}


	for (auto it = m_Ripples.begin(); it != m_Ripples.end(); )
	{
		if (!(*it)->update(*this))
		{
			it = m_Ripples.erase(it);
		}
		else
		{
			it++;
		}
	}

	for (auto& tileRegion : m_Regions)
	{
		auto& objects = tileRegion->getObjects();
		for (auto it = objects.begin(); it != objects.end(); )
		{
			if ((*it)->shouldDestroy())
			{
				it = objects.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
}

void Region::render(Renderer& renderer)
{
	renderer.begin();
	for (auto& tileRegion : m_Regions)
	{
		tileRegion->submit(renderer);
	}
	renderer.end();
	renderer.flush();
}

void Region::reloadTileUV(int x, int y)
{
	auto& tileRegion = getTileRegion(x, y);
	
	tileRegion->setUV(x, y, m_Tiles);
}

std::unique_ptr<TileRegion>& Region::getTileRegion(int x, int y)
{
	int rx = (int)std::floor((float)x / m_SubRegionWidth);
	int ry = (int)std::floor((float)y / m_SubRegionHeight);

	for (auto& tileRegion : m_Regions)
	{
		if (tileRegion->indexX() == rx && tileRegion->indexY() == ry)
		{
			return tileRegion;
		}
	}

	return m_Regions[0];
}
