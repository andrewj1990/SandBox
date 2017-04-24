#include "Level2D.h"

Level2D::Level2D()
	: m_PointLight(), 
	m_Background(glm::vec3(0, 0, -(Settings::Instance().Z_PLANE) + 10), glm::vec2(Window::Instance().getWidth(), Window::Instance().getHeight()), TextureManager::get("Textures/Level/bg.png")), 
	m_EntityManager(),
	m_Region(),
	aoe_test(glm::vec3(0, 0, 0), glm::vec2(100, 100), TextureManager::get("Textures/aoe.png"))
{
	init();
	aoe_test.setColor(glm::vec4(0, 0, 0, 1));
	m_WaterRippleTime = 0; 
}

void Level2D::init()
{
	const Camera& cam = Window::Instance().getCamera();
	m_Player = std::unique_ptr<Player>(new Player(Window::Instance().getWidth() / 2 - 16.0f, Window::Instance().getHeight() / 2 - 16.0f));

	for (int i = 0; i < 10; i++)
	{
		//m_Mobs.push_back(std::make_shared<BasicMob>(100, 100, m_Player));
		m_Mobs.push_back(std::make_shared<ProjectileMob>(200, 200, m_Player));
	}

	//TEntity mob = TEntity();
	//mob.attach<PositionComponent>(std::make_shared<PositionComponent>(glm::vec3(Window::Instance().getWidth() / 2 - 16.0f, Window::Instance().getHeight() / 2 - 16.0f, -(Window::Instance().getHeight() / 2 - 16.0f))));
	//mob.attach<VelocityComponent>(std::make_shared<VelocityComponent>(1, 1));
	//mob.attach<InputComponent>(std::make_shared<InputComponent>());
	//mob.attach<SpriteComponent>(std::make_shared<SpriteComponent>(glm::vec2(32, 32), TextureManager::get("Textures/Tree.png")));
	//m_EntityManager.add(mob);
}


void Level2D::update(float timeElapsed)
{
	m_EntityManager.update(timeElapsed);

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
	ObjectManager::init(BoundingBox(camX - qTreeOffset, camY - qTreeOffset, Settings::Instance().PROJECTION_WIDTH + qTreeOffset * 2, Settings::Instance().PROJECTION_HEIGHT + qTreeOffset * 2));

	//m_Region.addTiles(m_QTree);
	//m_Region.addTiles(m_QuadTree);
	m_Region.addObjects(ObjectManager::ObjectsQT);
	m_Region.addWaterTiles(ObjectManager::WaterTilesQT);

	for (auto& it = m_Mobs.begin(); it != m_Mobs.end(); )
	{
		if ((*it)->shouldDestroy())
		{
			it = m_Mobs.erase(it);
		}
		else
		{
			ObjectManager::ObjectsQT->insert(*it);
			(*it)->update(timeElapsed);

			++it;
		}
	}

	m_Player->update(m_Region, timeElapsed);
	m_PointLight.update(m_Player->getCenterX(), m_Player->getY() + 3, timeElapsed);

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

	if (Window::Instance().isButtonPressed(GLFW_MOUSE_BUTTON_3))
	{
		auto mx = Window::Instance().getMouseWorldPosX();
		auto my = Window::Instance().getMouseWorldPosY();
		for (int i = 0; i < 200; i++)
		{
			float offsetX = Utils::random(-200, 200);
			float offsetY = Utils::random(-200, 200);
			m_FireParticles.push_back(std::make_unique<Particle>(mx + offsetX, my + offsetY, Utils::random(5, 20), 90.0f));
		}
		//m_Mobs.push_back(std::make_shared<BasicMob>(mx, my, m_Player));
		//std::cout << "mobs : " << m_Mobs.size() << "\n";
		//std::cout << "number of fire particles : " << m_FireParticles.size() << "\n";
	}

	for (auto i = m_FireParticles.begin(); i != m_FireParticles.end(); )
	{
		if ((*i)->shouldDestroy())
		{
			i = m_FireParticles.erase(i);
		}
		else
		{
			(*i)->update(timeElapsed);
			i++;
		}
	}

	if (Window::Instance().isKeyPressed(GLFW_KEY_1))
	{
		aoe_test.setSize(aoe_test.getSize() - glm::vec2(1));
	}
	if (Window::Instance().isKeyPressed(GLFW_KEY_2))
	{
		aoe_test.setSize(aoe_test.getSize() + glm::vec2(1));
	}
	if (Window::Instance().isKeyPressed(GLFW_KEY_3))
	{
		aoe_test.setColor(aoe_test.getColour() + glm::vec4(0.01f, 0, 0, 0));
	}

	auto mx = Window::Instance().getMouseWorldPosX() - aoe_test.getSize().x / 2;
	auto my = Window::Instance().getMouseWorldPosY() - aoe_test.getSize().y / 2;

	aoe_test.setPosition(mx, my);

	if (Window::Instance().isKeyPressed(GLFW_KEY_M))
	{
//		ParticleManager::instance().add(Entity(glm::vec3(mx, my, 0), glm::vec2(32), glm::vec4(1, 0, 0, 1)));
		ParticleManager::instance().add(WaterParticle(mx, my, Utils::random(0, 360), 100));
	}

	ParticleManager::instance().update(timeElapsed);

}

void Level2D::render(Renderer& renderer)
{
	m_EntityManager.render(renderer);

	renderer.render(m_Background);
	m_Region.render(renderer);

	renderer.begin();
	renderer.m_AlphaTest = false;
	for (auto& waterRipple : m_WaterRipples) waterRipple->submit(renderer);
	for (auto& fireParticle : m_FireParticles) fireParticle->submit(renderer);
	renderer.end();
	renderer.flush();
	renderer.m_AlphaTest = true;


	for (auto& mob : m_Mobs)
	{
		mob->render(renderer);
	}

	m_Player->render(renderer);

	ParticleManager::instance().render(renderer);


	if (Settings::Instance().debugShowCollisionBoxes)
	{
		auto mx = Window::Instance().getMouseWorldPosX();
		auto my = Window::Instance().getMouseWorldPosY();
		BoundingBox mouseBoundingBox(mx-8, my-8, 16, 16);

		renderer.render(Renderable(glm::vec3(mx - 8, my - 8, 0), glm::vec2(16), TextureManager::get("Textures/collision_box.png")));
		std::vector<std::shared_ptr<Sprite>> tiles;
		ObjectManager::QuadTree->retrieve(tiles, mouseBoundingBox);
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
		ObjectManager::ObjectsQT->getBounds(boundingBoxes);

		for (auto& bb : boundingBoxes)
		{
			renderer.debugRender(bb, TextureManager::get("Textures/bbox.png"));
		}

		std::vector<std::shared_ptr<Entity>> m_Data;
		ObjectManager::ObjectsQT->retrieve(m_Data, m_PointLight.getLightRegion());

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

	renderer.m_AlphaTest = false;
	//renderer.render(aoe_test);
	renderer.m_AlphaTest = true;
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

	for (auto& fireParticle : m_FireParticles) fireParticle->submit(renderer);

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
