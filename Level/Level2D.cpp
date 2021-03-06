#include "Level2D.h"

Level2D::Level2D()
	: m_PointLight(),
	m_Background(glm::vec3(0, 0, -(Settings::Instance().Z_PLANE) + 10), glm::vec2(Window::Instance().getWidth(), Window::Instance().getHeight()), TextureManager::get("Textures/Level/bg.png")),
	m_EntityManager(),
	m_Region(),
	aoe_test(glm::vec3(0, 0, 0), glm::vec2(100, 100), TextureManager::get("Textures/aoe.png")),
	m_MobSpawnTimer(),
	m_MobSpawnTime(0),
	particleFBO(std::make_unique<FrameBuffer>(10000, 10000))
{
	//particleFBO = std::make_unique<FrameBuffer>();
	init();
	aoe_test.setColor(glm::vec4(0, 0, 0, 1));
	m_WaterRippleTime = 0; 
}

void Level2D::init()
{
	const Camera& cam = Window::Instance().getCamera();
	m_Player = std::unique_ptr<Player>(new Player(Window::Instance().getWidth() / 2 - 16.0f, Window::Instance().getHeight() / 2 - 16.0f));

	for (int i = 0; i < 5; i++)
	{
		//m_Mobs.push_back(std::make_shared<MeleeMob>(Utils::random(1, 1000), Utils::random(1,1000), m_Player));
	}

	// create box around player
	load();

	m_MobSpawnTimer.reset();
	m_MobSpawnTime = 1.0f;

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
	//ObjectManager::init(BoundingBox(camX - qTreeOffset, camY - qTreeOffset, Settings::Instance().PROJECTION_WIDTH + qTreeOffset * 2, Settings::Instance().PROJECTION_HEIGHT + qTreeOffset * 2));
	ObjectManager::init(BoundingBox(-5000, -5000, 10000, 10000));
	//ObjectManager::init(BoundingBox(m_Player->getCenterX() - Settings::Instance().PROJECTION_WIDTH, m_Player->getCenterY() - Settings::Instance().PROJECTION_HEIGHT, 
	//	Settings::Instance().PROJECTION_WIDTH * 2, Settings::Instance().PROJECTION_HEIGHT * 2));

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
			ObjectManager::ObjectsQT->insert(it->get());
			ObjectManager::MobQT->insert(it->get());

			(*it)->update(timeElapsed);

			++it;
		}
	}

	for (auto& it = objects_.begin(); it != objects_.end(); ) {
		if ((*it)->shouldDestroy()) {
			it = objects_.erase(it);
		} else {
			ObjectManager::ObjectsQT->insert(it->get());
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
			//m_FireParticles.push_back(std::make_unique<Particle>(mx + offsetX, my + offsetY, Utils::random(5, 20), 90.0f));
		}
		//m_Mobs.push_back(std::make_shared<BasicMob>(mx, my, m_Player));
		//std::cout << "mobs : " << m_Mobs.size() << "\n";
		//std::cout << "number of fire particles : " << m_FireParticles.size() << "\n";
		m_Mobs.push_back(std::make_shared<MeleeMob>(mx, my, m_Player));

	}

	if (m_MobSpawnTimer.elapsed() > m_MobSpawnTime)
	{
		m_MobSpawnTimer.reset();
		std::vector<Entity*> objs;
		float mobX = m_Player->getCenterX() + Utils::random(-1000, 1000);
		float mobY = m_Player->getCenterY() + Utils::random(-1000, 1000);
		BoundingBox mobBbox(mobX, mobY, 32, 32);
		ObjectManager::ObjectsQT->retrieve(objs, mobBbox);
		bool collision = false;
		for (const auto& obj : objs) {
			if (obj->collide(mobBbox)) {
				collision = true;
				break;
			}
		}

		if (!collision) {
			m_Mobs.push_back(std::make_shared<MeleeMob>(mobX, mobY, m_Player));
		}
		//m_Mobs.push_back(std::make_shared<MeleeMob>(m_Player->getCenterX() + Utils::random(-1000, 1000), m_Player->getCenterY() + Utils::random(-1000, 1000), m_Player));
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
		for (int i = 0; i < 100; i++)
			ParticleManager::instance().add(FireProjectile(mx, my, glm::radians(Utils::random(0.0f, 360.0f))));
	}

	ParticleManager::instance().update(timeElapsed);

	if (Settings::Instance().timeModifier < 1.0f)
	{
		Settings::Instance().timeModifier += timeElapsed;
	}

	Settings::Instance().timeModifier = std::min(Settings::Instance().timeModifier, 1.0f);
}

void Level2D::render(Renderer& renderer, FrameBuffer* fbo)
{
	fbo->unbind();
	particleFBO->bind(false);
	glViewport(0, 0, particleFBO->getTexture()->getWidth(), particleFBO->getTexture()->getHeight());

	glm::mat4 mat;
	float aspectX = (float)Settings::Instance().PROJECTION_WIDTH / (float)particleFBO->getTexture()->getWidth();
	float aspectY = (float)Settings::Instance().PROJECTION_HEIGHT / (float)particleFBO->getTexture()->getHeight();
	//float pFBOx = m_Player->getCenterX() - Window::Instance().getWidth() / 2;
	//float pFBOy = m_Player->getCenterY() - Window::Instance().getHeight() / 2;
	float offset = 5000.0f;
	float cx = Window::Instance().getCamera().getPosition().x - m_Player->getCameraOffsetX();
	float cy = Window::Instance().getCamera().getPosition().y - m_Player->getCameraOffsetY();
	mat = glm::translate(mat, glm::vec3(Window::Instance().getCamera().getPosition().x, Window::Instance().getCamera().getPosition().y, 0));
	//mat = glm::translate(mat, glm::vec3(cx, cy, 0));
	//mat = glm::translate(mat, glm::vec3(m_Player->getCenterX() - Settings::Instance().PROJECTION_WIDTH / 2, m_Player->getCenterY() - Settings::Instance().PROJECTION_HEIGHT / 2, 0));
	mat = glm::scale(mat, glm::vec3(aspectX, aspectY, 0));
	mat = glm::translate(mat, glm::vec3(offset, offset, 0));
	//mat = glm::translate(mat, glm::vec3(-m_Player->getCameraOffsetX(), -m_Player->getCameraOffsetY(), 0));
	//std::cout << Window::Instance().getCamera().getPosition().x << ", " << Window::Instance().getCamera().getPosition().y << " | ";
	//std::cout << m_Player->getCameraOffsetX() << ", " << m_Player->getCameraOffsetY() << " | ";
	//std::cout << m_Player->getCenterX() << ", " << m_Player->getCenterY() << " | ";
	//std::cout << cx << ", " << cy << "\n";
	renderer.push(mat);
	renderer.seperate = true;
	ParticleManager::instance().renderOnce(renderer);
	//ParticleManager::instance().render(renderer);
	renderer.seperate = false;
	renderer.pop();

	particleFBO->unbind(false);
	fbo->bind();
	//glViewport(0, 0, fbo->getTexture()->getWidth(), fbo->getTexture()->getHeight());
	glViewport(0, 0, fbo->getTexture()->getWidth(), fbo->getTexture()->getHeight());

	m_EntityManager.render(renderer);

	//renderer.render(m_Background);
	m_Region.render(renderer);
 

	//renderer.render(Renderable(glm::vec3(Window::Instance().getCamera().getPosition().x, Window::Instance().getCamera().getPosition().y, 0), glm::vec2(particleFBO->getTexture()->getWidth(), particleFBO->getTexture()->getHeight()), particleFBO->getTexture()));
	renderer.m_SrcFactor = GL_ONE;
	renderer.render(Renderable(glm::vec3(-offset, -offset, 0), glm::vec2(particleFBO->getTexture()->getWidth(), particleFBO->getTexture()->getHeight()), particleFBO->getTexture()));
	renderer.m_SrcFactor = GL_SRC_ALPHA;

	ParticleManager::instance().render(renderer);

	renderer.begin();
	renderer.m_AlphaTest = false;
	for (auto& waterRipple : m_WaterRipples) waterRipple->submit(renderer);
	for (auto& fireParticle : m_FireParticles) fireParticle->submit(renderer);
	for (auto& object : objects_) object->submit(renderer);
	renderer.end();
	renderer.flush();
	renderer.m_AlphaTest = true;
	
	renderer.begin();
	for (auto& mob : m_Mobs)
	{
		mob->submit(renderer);
	}
	renderer.end();
	renderer.flush();

	ResourceManager::getInstance().shader("basic_shader")->setUniform("outline", true);
	m_Player->render(renderer);
	ResourceManager::getInstance().shader("basic_shader")->setUniform("outline", false);

	m_Region.renderObjects(renderer);

	if (Settings::Instance().debugShowCollisionBoxes)
	{
		auto mx = Window::Instance().getMouseWorldPosX();
		auto my = Window::Instance().getMouseWorldPosY();
		BoundingBox mouseBoundingBox(mx-8, my-8, 16, 16);

		renderer.render(Renderable(glm::vec3(mx - 8, my - 8, 0), glm::vec2(16), TextureManager::get("Textures/collision_box.png")));
		//std::vector<Sprite*> tiles;
		//ObjectManager::QuadTree->retrieve(tiles, mouseBoundingBox);
		std::vector<Entity*> entities;
		ObjectManager::ObjectsQT->retrieve(entities, mouseBoundingBox);
		renderer.begin();
		for (auto& t : entities)
		{
			renderer.submit(Renderable(glm::vec3(t->getCollisionBox().x, t->getCollisionBox().y, 0), glm::vec2(t->getCollisionBox().width, t->getCollisionBox().height), TextureManager::get("Textures/collision_box.png")));
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

		std::vector<Entity*> m_Data;
		ObjectManager::ObjectsQT->retrieve(m_Data, m_PointLight.getLightRegion());

		for (auto sprite : m_Data)
		{
			renderer.debugRender((sprite->getCollisionBox()), TextureManager::get("Textures/collision_box.png"));
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

void Level2D::load()
{
	std::string param_name;
	std::string texture_name;
	bool solid;
	float x;
	float y;
	float w;
	float h;

	std::ifstream level("level.txt");

	while (level >> param_name) {
		level >> param_name >> texture_name;
		level >> param_name >> x >> y;
		level >> param_name >> w >> h;

		std::unique_ptr<Entity> object = std::make_unique<Entity>(glm::vec3(x, y, 0), glm::vec2(w, h), TextureManager::get(texture_name));
		level >> param_name >> solid;
		object->setSolid(solid);
		level >> param_name >> x >> y >> w >> h;
		object->getCollisionBox().x = x;
		object->getCollisionBox().y = y;
		object->getCollisionBox().width = w;
		object->getCollisionBox().height = h;
		object->getOccluder().x = x;
		object->getOccluder().y = y;
		object->getOccluder().width = w;
		object->getOccluder().height = h;
		objects_.push_back(std::move(object));
	}

	std::cout << "objects : " << objects_.size() << "\n";

	level.close();
}
