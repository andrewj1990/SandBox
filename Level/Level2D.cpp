#include "Level2D.h"

Level2D::Level2D()
	: m_Light(), m_Background(glm::vec3(0, 0, 0), glm::vec2(Window::Instance().getWidth(), Window::Instance().getHeight()), TextureManager::get("Textures/left.jpg"))
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

	m_Delay = 0;
}

void Level2D::init()
{
	const Camera& cam = Window::Instance().getCamera();
	m_Player = std::unique_ptr<Player>(new Player(Window::Instance().getWidth() / 2 - 16.0f, Window::Instance().getHeight() / 2 - 16.0f));

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

	m_Background.setPosition(cam.Position.x, cam.Position.y);
	
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


	int camX = Window::Instance().getCamera().Position.x;
	int camY = Window::Instance().getCamera().Position.y;
	int winW = Window::Instance().getWidth();
	int winH = Window::Instance().getHeight();
	//m_Light.update(camX + Window::Instance().mouseX(), camY + (winH - Window::Instance().mouseY()), timeElapsed);
	m_Light.update(m_Player->getCenterX(), m_Player->getCenterY(), timeElapsed);

	m_Delay++;
	if (Window::Instance().isButtonPressed(GLFW_MOUSE_BUTTON_1) && m_Delay > 100)
	{
		m_Delay = 0;
		m_Lights.push_back(Light(m_Light));
	}

	if (Window::Instance().isButtonPressed(GLFW_MOUSE_BUTTON_1))
	{
		float mx = camX + Window::Instance().mouseX();
		float my = camY + (Window::Instance().getHeight() - Window::Instance().mouseY());

		std::string positionInRegion = std::to_string((int)mx / 16 * 16) + "_" + std::to_string((int)my / 16 * 16);
		auto it = m_RegionSet.find(positionInRegion);
		if (it == m_RegionSet.end())
		{
			m_RegionSet.insert(positionInRegion);
			// remove the tile from the region

		}

	}

	//m_QuadTree = std::unique_ptr<QuadTree>(new QuadTree(0, BoundingBox(camX, camY, winW, winH)));
	m_QTree = std::unique_ptr<QTree<Renderable>>(new QTree<Renderable>(0, BoundingBox(camX, camY, winW, winH)));

	for (auto& tileRegion : m_TestRegion)
	{
		for (auto& tile : tileRegion->getTiles())
		{
			//m_QuadTree->insert(tile.get());
			tile->setColor(glm::vec4(1, 1, 1, 1));
			m_QTree->insert(tile.get());
		}
	}


	std::vector<Renderable*> m_Data;
	m_QTree->retrieve(m_Data, m_Light.getLightRegion());

	m_Light.update(m_Data, timeElapsed);
}

void Level2D::render(Renderer& renderer)
{
	renderer.render(m_Background);

	for (auto& tileRegion : m_TestRegion)
	{
		tileRegion->render(renderer);
	}

	m_Player->render(renderer);
}

void Level2D::renderLights(Renderer& renderer)
{
	ResourceManager::getInstance().shader("lightShadow")->use();

	renderer.begin();
	m_Light.renderShadow(renderer);

	for (auto& light : m_Lights)
	{
		light.renderShadow(renderer);
	}

	renderer.end();
	renderer.flush(GL_SRC_ALPHA, GL_ONE);

	ResourceManager::getInstance().shader("basic_shader")->use();
}

void Level2D::addTileRegion(int i, int j)
{
	m_TestRegion.push_back(std::unique_ptr<TileRegion>(new TileRegion(i, j)));
	m_TestRegion.back()->init(m_RegionSet);
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
