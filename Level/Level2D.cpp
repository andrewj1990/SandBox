#include "Level2D.h"

Level2D::Level2D()
	: m_PointLight(), m_Background(glm::vec3(0, 0, -(Settings::Instance().Z_PLANE) + 10), glm::vec2(Window::Instance().getWidth(), Window::Instance().getHeight()), TextureManager::get("Textures/Level/bg.png")), m_Region()
{
	int camX = (int)Window::Instance().getCamera().Position.x;
	int camY = (int)Window::Instance().getCamera().Position.y;
	int winW = (int)Window::Instance().getWidth();
	int winH = (int)Window::Instance().getHeight();
	m_WaterTilesQT = std::unique_ptr<QTree<Sprite>>(new QTree<Sprite>(0, BoundingBox(camX, camY, winW, winH)));
	m_QuadTree = std::unique_ptr<QTree<Sprite>>(new QTree<Sprite>(0, BoundingBox(camX, camY, winW, winH)));
	m_ObjectsQT = std::unique_ptr<QTree<Sprite>>(new QTree<Sprite>(0, BoundingBox(camX, camY, winW, winH)));
	m_TestQT = std::unique_ptr<QTree<Sprite>>(new QTree<Sprite>(0, BoundingBox((int)Window::Instance().getCamera().Position.x, (int)Window::Instance().getCamera().Position.y, (int)Window::Instance().getWidth(), (int)Window::Instance().getHeight())));

	init();

	m_WaterRippleTime = 0; 
}

void Level2D::init()
{
	const Camera& cam = Window::Instance().getCamera();
	m_Player = std::unique_ptr<Player>(new Player(Window::Instance().getWidth() / 2 - 16.0f, Window::Instance().getHeight() / 2 - 16.0f));
}

void Level2D::update(float timeElapsed)
{
	Camera& cam = Window::Instance().getCamera();

	float px = m_Player->getCenterX();
	float py = m_Player->getCenterY();
	float ccx = px - Settings::Instance().PROJECTION_WIDTH / 2.0f;
	float ccy = py - Settings::Instance().PROJECTION_HEIGHT / 2.0f;
	m_Background.setPosition(ccx + m_Player->getCameraOffsetX(), ccy + m_Player->getCameraOffsetY());

	m_Region.update(timeElapsed);

	int camX = Window::Instance().getCamera().Position.x;
	int camY = Window::Instance().getCamera().Position.y;
	int winW = Window::Instance().getWidth();
	int winH = Window::Instance().getHeight();

	if (Window::Instance().isKeyTyped(GLFW_KEY_J))
	{
		m_PointLights.push_back(PointLight(m_PointLight));
	}
	if (Window::Instance().isKeyPressed(GLFW_KEY_Y))
	{
		m_PointLights.push_back(PointLight(m_PointLight));
	}

	int qTreeOffset = 200;
	m_WaterTilesQT	= std::unique_ptr<QTree<Sprite>>(new QTree<Sprite>(0, BoundingBox(camX - qTreeOffset, camY - qTreeOffset, Settings::Instance().PROJECTION_WIDTH + qTreeOffset * 2, Settings::Instance().PROJECTION_HEIGHT + qTreeOffset * 2)));
	m_QuadTree		= std::unique_ptr<QTree<Sprite>>(new QTree<Sprite>(0, BoundingBox(camX - qTreeOffset, camY - qTreeOffset, Settings::Instance().PROJECTION_WIDTH + qTreeOffset * 2, Settings::Instance().PROJECTION_HEIGHT + qTreeOffset * 2)));
	m_ObjectsQT		= std::unique_ptr<QTree<Sprite>>(new QTree<Sprite>(0, BoundingBox(camX - qTreeOffset, camY - qTreeOffset, Settings::Instance().PROJECTION_WIDTH + qTreeOffset * 2, Settings::Instance().PROJECTION_HEIGHT + qTreeOffset * 2)));

	//m_Region.addTiles(m_QTree);
	//m_Region.addTiles(m_QuadTree);
	m_Region.addObjects(m_ObjectsQT);
	m_Region.addWaterTiles(m_WaterTilesQT);

	m_Player->update(m_Region, m_ObjectsQT, m_WaterTilesQT, timeElapsed);
	m_PointLight.update(m_Player->getCenterX(), m_Player->getY() + 3, m_ObjectsQT, timeElapsed);

	// water ripples
	m_WaterRippleTime += timeElapsed;
	if (m_WaterRippleTime > 10000.0f) m_WaterRippleTime = 0.0f;
	if (m_Region.getTileType(m_Player->getCenterX(), m_Player->getCenterY()) == TileType::SHALLOW_WATER)
	{
		if (m_Player->isMoving() && m_WaterRippleTime > 0.1f)
		{
			m_WaterRippleTime = 0.0f;
			float numRipples = Utils::random(1, 4);
			for (int i = 0; i < numRipples; i++)
			{
				m_WaterRipples.push_back(std::unique_ptr<WaterRipple>(new WaterRipple(m_Player->getCenterX(), m_Player->getCenterY(), Utils::random(100, 300))));
			}
		}
		else if (m_WaterRippleTime > 0.7f)
		{
			m_WaterRippleTime = 0.0f;
			m_WaterRipples.push_back(std::unique_ptr<WaterRipple>(new WaterRipple(m_Player->getCenterX(), m_Player->getCenterY(), Utils::random(30, 50))));
		}
	}

	for (auto it = m_WaterRipples.begin(); it != m_WaterRipples.end(); )
	{
		if ((*it)->complete())
		{
			it = m_WaterRipples.erase(it);
		}
		else
		{
			(*it)->update(m_Region, timeElapsed);
			it++;
		}
	}

	ObjectManager::ObjectsQT = std::shared_ptr<QTree<Sprite>>(new QTree<Sprite>(0, BoundingBox(-qTreeOffset, -qTreeOffset, (int)Window::Instance().getWidth() + qTreeOffset * 2, (int)Window::Instance().getHeight() + qTreeOffset * 2)));

	const auto& win = Window::Instance();
	if (win.isButtonPressed(GLFW_MOUSE_BUTTON_3))
	{
		m_TestObjects.push_back(std::shared_ptr<Sprite>(new Sprite(glm::vec3(win.getMouseWorldPosX(), win.getMouseWorldPosY(), Settings::Instance().Z_PLANE - 1), glm::vec2(10, 10), TextureManager::get("Textures/container2_specular.png"))));
	}

	for (auto& testObj : m_TestObjects)
	{
		ObjectManager::ObjectsQT->insert(testObj);
	}
}

void Level2D::render(Renderer& renderer)
{
	renderer.render(m_Background);
	m_Region.render(renderer);

	renderer.begin();
	renderer.m_AlphaTest = false;
	for (auto& waterRipple : m_WaterRipples) waterRipple->submit(renderer);
	renderer.end();
	renderer.flush();
	renderer.m_AlphaTest = true;

	m_Player->render(renderer);

	if (Settings::Instance().debugShowCollisionBoxes)
	{
		auto mx = Window::Instance().getMouseWorldPosX();
		auto my = Window::Instance().getMouseWorldPosY();
		BoundingBox mouseBoundingBox(mx-8, my-8, 16, 16);

		renderer.render(Renderable(glm::vec3(mx - 8, my - 8, 0), glm::vec2(16), TextureManager::get("Textures/collision_box.png")));
		std::vector<std::shared_ptr<Sprite>> tiles;
		m_QuadTree->retrieve(tiles, mouseBoundingBox);
		renderer.begin();
		for (auto& t : tiles)
		{
			renderer.submit(Renderable(glm::vec3(t->getCollisionBox()->x, t->getCollisionBox()->y, 0), glm::vec2(t->getCollisionBox()->width, t->getCollisionBox()->height), TextureManager::get("Textures/collision_box.png")));
		}
		renderer.end();
		renderer.flush();
	}

	// quadtree outline
	if (Settings::Instance().debugShowQuadTree)
	{
		std::vector<BoundingBox> boundingBoxes;
		m_ObjectsQT->getBounds(boundingBoxes);

		for (auto& bb : boundingBoxes)
		{
			renderer.debugRender(bb, TextureManager::get("Textures/bbox.png"));
		}

		std::vector<std::shared_ptr<Sprite>> m_Data;
		m_ObjectsQT->retrieve(m_Data, m_PointLight.getLightRegion());

		for (auto sprite : m_Data)
		{
			renderer.debugRender(*(sprite->getCollisionBox()), TextureManager::get("Textures/collision_box.png"));
		}
	}

	// light outline
	if (Settings::Instance().debugShowLightRange)
	{
		renderer.debugRender(m_PointLight.getLightRegion(), TextureManager::get("Textures/bbox.png"));
	}


	// testing qtree
	std::vector<BoundingBox> boundingBoxes;
	ObjectManager::ObjectsQT->getBounds(boundingBoxes);

	for (auto& bb : boundingBoxes)
	{
		renderer.debugRender(bb, TextureManager::get("Textures/bbox.png"));
	}

	renderer.begin();
	for (auto& testObj : m_TestObjects)
	{
		renderer.submit(*testObj);
	}

	auto mx = Window::Instance().getMouseWorldPosX();
	auto my = Window::Instance().getMouseWorldPosY();
	int tempBounds = 8;
	BoundingBox mouseBoundingBox(mx - tempBounds, my - tempBounds, tempBounds * 2, tempBounds * 2);
	std::vector<std::shared_ptr<Sprite>> m_Data;
	ObjectManager::ObjectsQT->retrieve(m_Data, mouseBoundingBox);
	renderer.end();
	renderer.flush();

	for (auto& sprite : m_Data)
	{
		renderer.debugRender(*(sprite->getCollisionBox()), TextureManager::get("Textures/collision_box.png"));
	}

	renderer.debugRender(mouseBoundingBox, TextureManager::get("Textures/collision_box.png"));


	//ResourceManager::getInstance().shader("outline_shader")->use();
	//renderer.begin();
	//renderer.m_AlphaTest = false;

	//std::vector<std::shared_ptr<Sprite>> m_Data2;
	//m_ObjectsQT->retrieve(m_Data2, mouseBoundingBox);

	//for (auto& sprite : m_Data2)
	//{
	//	renderer.submit(*sprite);
	//}

	//renderer.end();
	//renderer.flush();
	//renderer.m_AlphaTest = true;

	//ResourceManager::getInstance().shader("basic_shader")->use();
}

void Level2D::renderLights(Renderer& renderer)
{
	ResourceManager::getInstance().shader("lightShadow")->use();

	renderer.begin();
	renderer.m_BlendFactor = GL_ONE;
	renderer.m_AlphaTest = false;

	m_PointLight.render(renderer);

	for (auto& pointLight : m_PointLights)
	{
		pointLight.render(renderer);
	}

	m_Player->renderLight(renderer);

	renderer.end();
	renderer.flush();
	
	renderer.m_BlendFactor = GL_ONE_MINUS_SRC_ALPHA;
	renderer.m_AlphaTest = true;

	ResourceManager::getInstance().shader("basic_shader")->use();
}

void Level2D::waterRipple(int x, int y)
{
	float numRipples = Utils::random(1, 4);
	for (int i = 0; i < numRipples; i++)
	{
		m_WaterRipples.push_back(std::unique_ptr<WaterRipple>(new WaterRipple(x, y, Utils::random(100, 300))));
	}
}
