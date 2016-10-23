#include "Level2D.h"

Level2D::Level2D()
	: m_Light()
{
	m_RegionX = 0;
	m_RegionY = 0;
	m_RegionSizeX = 6;
	m_RegionSizeY = 4;

	int camX = Window::Instance().getCamera().Position.x;
	int camY = Window::Instance().getCamera().Position.y;
	int winW = Window::Instance().getWidth();
	int winH = Window::Instance().getHeight();
	m_QuadTree = std::unique_ptr<QuadTree>(new QuadTree(0, BoundingBox(camX, camY, winW, winH)));
	m_QTree = std::unique_ptr<QTree<Renderable>>(new QTree<Renderable>(0, BoundingBox(camX, camY, winW, winH)));

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

	for (int i = 0; i < m_RegionSizeX; i++)
	{
		for (int j = 0; j < m_RegionSizeY; j++)
		{
			addTileRegion(i, j);
		}
	}

}

void Level2D::update(float timeElapsed)
{
	int tileSize = 16;
	int numTilePerRegion = 16;
	int regionSize= tileSize * numTilePerRegion;
	const Camera& cam = Window::Instance().getCamera();
	int regionX = (int)std::floor((cam.Position.x / regionSize));
	int regionY = (int)std::floor((cam.Position.y / regionSize));
	//std::cout << "camPos : " << regionX << ", " << regionY << "\n";
	
	// region changed
	if (m_RegionX != regionX)
	{
		int index = m_RegionX < regionX ? m_RegionX : m_RegionX + m_RegionSizeX - 1;
		removeTileRegionCol(index);
		int indexAdd = m_RegionX > regionX ? regionX : m_RegionX + m_RegionSizeX;
		addTileRegionCol(indexAdd);
		m_RegionX = regionX;
	}
	if (m_RegionY != regionY)
	{
		int index = m_RegionY < regionY ? m_RegionY : m_RegionY + m_RegionSizeY - 1;
		removeTileRegionRow(index);
		int indexAdd = m_RegionY > regionY ? regionY : m_RegionY + m_RegionSizeY;
		addTileRegionRow(indexAdd);
		m_RegionY = regionY;
	}

	m_Player->update(timeElapsed);

	m_Light.update(timeElapsed);

	int camX = Window::Instance().getCamera().Position.x;
	int camY = Window::Instance().getCamera().Position.y;
	int winW = Window::Instance().getWidth();
	int winH = Window::Instance().getHeight();
	//m_QuadTree = std::unique_ptr<QuadTree>(new QuadTree(0, BoundingBox(camX, camY, winW, winH)));
	m_QTree = std::unique_ptr<QTree<Renderable>>(new QTree<Renderable>(0, BoundingBox(camX, camY, winW, winH)));

	for (auto& tileRegion : m_TestRegion)
	{
		for (auto& tile : tileRegion->getTiles())
		{
			//m_QuadTree->insert(tile.get());
			m_QTree->insert(tile.get());
		}
	}

}

void Level2D::render(Renderer& renderer)
{
	//renderer.render(m_Tiles);
	for (auto& tileRegion : m_TestRegion)
	{
		//tileRegion->render(renderer);
	}

	m_Player->render(renderer);

	std::vector<Renderable*> m_Data;
	m_QTree->retrieve(m_Data, m_Light.getLightRegion());

	renderer.render(m_Data);

	m_Light.render(renderer);
}

void Level2D::addTileRegion(int i, int j)
{
	m_TestRegion.push_back(std::unique_ptr<TileRegion>(new TileRegion(i, j)));
	m_TestRegion.back()->init();
}

void Level2D::addTileRegionRow(int index, bool wholeRow)
{
	for (int i = m_RegionX; i < m_RegionX + m_RegionSizeX; i++)
	{
		addTileRegion(i, index);
	}
}

void Level2D::addTileRegionCol(int index, bool wholeCol)
{
	for (int i = m_RegionY; i < m_RegionY + m_RegionSizeY; i++)
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
