#include "Level2D.h"

Level2D::Level2D()
{
	m_RegionX = 0;
	m_RegionY = 0;
	m_RegionSize = 5;

	init();
}

void Level2D::init()
{
	const Camera& cam = Window::Instance().getCamera();
	m_Player = std::unique_ptr<Player>(new Player(Window::Instance().getWidth() / 2 - 16.0f, Window::Instance().getHeight() / 2 - 16.0f));

	constexpr int r_size = 100;
	constexpr int c_size = 100;
	int tiles[r_size][c_size]; // = {
	//	{0, 0, 0, 0, 0, 0, 0, 0, 0},
	//	{0, 0, 0, 0, 0, 0, 0, 0, 1},
	//	{0, 1, 1, 0, 0, 1, 1, 0, 0},
	//	{0, 1, 1, 1, 1, 1, 1, 1, 1},
	//	{1, 1, 1, 1, 1, 1, 1, 1, 1}
	//};

	//int tile_size = 16;

	for (int row = 0; row < r_size; row++) {
		for (int col = 0; col < c_size; col++) {
			int i = Utils::random(0, 2);
			tiles[row][col] = i;
			//m_Tiles.push_back(std::unique_ptr<Tile>(new Tile("Textures/Level/Tiles_2.png")));
			//auto& t = m_Tiles[col + row * c_size];
			//if (i != 0) {
			//	t->init(col * tile_size, (r_size - row) * tile_size, glm::vec4(0, 1, 0, 1), false, false);
			//}
			//else
			//{
			//	t->init(col * tile_size, (r_size - row) * tile_size, glm::vec4(1, 1, 0, 0), false, false);
			//}
		}
	}

	int midX = (int)(Window::Instance().getWidth() / 2) / (16 * 16);
	int midY = (int)(Window::Instance().getHeight() / 2) / (16 * 16);

	midX = 1;
	midY = 1;

	std::cout << "m : " << midX << ", " << midY << "\n";

	for (int i = 0; i < m_RegionSize; i++)
	{
		for (int j = 0; j < m_RegionSize; j++)
		{
			addTileRegion(i, j);
		}
	}

}

void Level2D::update(float timeElapsed)
{
	const Camera& cam = Window::Instance().getCamera();
	//std::cout << "camPos : " << (int)(cam.Position.x / (16 * 16)) << ", " << (int)(cam.Position.y / (16 * 16)) << "\n";
	int regionX = (int)(cam.Position.x / (16 * 16));
	int regionY = (int)(cam.Position.y / (16 * 16));

	// region changed
	if (m_RegionX != regionX)
	{
		int index = m_RegionX < regionX ? m_RegionX : m_RegionX + m_RegionSize - 1;
		removeTileRegionCol(index);
		int indexAdd = m_RegionX > regionX ? regionX : m_RegionX + m_RegionSize;
		addTileRegionCol(indexAdd);
		m_RegionX = regionX;
	}
	if (m_RegionY != regionY)
	{
		int index = m_RegionY < regionY ? m_RegionY : m_RegionY + m_RegionSize - 1;
		removeTileRegionRow(index);
		int indexAdd = m_RegionY > regionY ? regionY : m_RegionY + m_RegionSize;
		addTileRegionRow(indexAdd);
		m_RegionY = regionY;
	}

	m_Player->update(timeElapsed);

}

void Level2D::render(Renderer& renderer)
{
	//renderer.render(m_Tiles);
	for (auto& tileRegion : m_TestRegion)
	{
		tileRegion->render(renderer);
	}

	m_Player->render(renderer);
}

void Level2D::addTileRegion(int i, int j)
{
	m_TestRegion.push_back(std::unique_ptr<TileRegion>(new TileRegion(i, j)));
	m_TestRegion.back()->init();
}

void Level2D::addTileRegionRow(int index, bool wholeRow)
{
	for (int i = m_RegionX; i < m_RegionX + m_RegionSize; i++)
	{
		addTileRegion(i, index);
	}
}

void Level2D::addTileRegionCol(int index, bool wholeCol)
{
	for (int i = m_RegionY; i < m_RegionY + m_RegionSize; i++)
	{
		addTileRegion(index, i);
	}
}

void Level2D::removeTileRegionRow(int index, bool wholeRowCol)
{
	for (auto it = m_TestRegion.begin(); it != m_TestRegion.end(); )
	{
		if ((*it)->indexY() == index)
		{
			it = m_TestRegion.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void Level2D::removeTileRegionCol(int index, bool wholeRowCol)
{
	for (auto it = m_TestRegion.begin(); it != m_TestRegion.end(); )
	{
		if ((*it)->indexX() == index)
		{
			it = m_TestRegion.erase(it);
		}
		else
		{
			++it;
		}
	}
}
