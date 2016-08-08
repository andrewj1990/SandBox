#include "Level.h"

Level::Level()
	: m_Player(Window::Instance().getWidth() / 2 - 16.0f, Window::Instance().getHeight() / 2 - 16.0f)
{
	treeSprite = Sprite(glm::vec3(600, 400, 0), glm::vec2(64, 64), TextureManager::get("Textures/Tree.png"));

	m_PlayerPtr = std::shared_ptr<Player>(new Player(Window::Instance().getWidth() / 2 - 16.0f, Window::Instance().getHeight() / 2 - 16.0f));
	m_Entities.push_back(m_PlayerPtr);
	//m_Entities.push_back(std::unique_ptr<Player>(new Player(Window::Instance().getWidth() / 2 - 16.0f, Window::Instance().getHeight() / 2 - 16.0f)));
	for (int i = 0; i < 1000; i++)
	{
		m_Entities.push_back(std::unique_ptr<Entity>(new Entity(glm::vec3(Utils::random(400, 10000), Utils::random(0, 10000), 0), glm::vec2(64, 64), TextureManager::get("Textures/Tree.png"))));
	}

	init();
}

void Level::init()
{
	int y = 0;
	for (int x = 0; x < 1280 / 32; x++)
	{
		y = (int)m_Noise.scaledOctaveNoise(5, 0.5, 1, 0, 720, x * 1.0f, 0);
		m_Platforms.push_back(Renderable(glm::vec3(x * 32, y, 0), glm::vec2(32, 32), TextureManager::get("Textures/Level/Terrain.png")));
	}

	m_Enemies.push_back(std::unique_ptr<Boss>(new Boss(m_Player, 600, 600)));
	//for (int i = 0; i < 11; i++)
	//{
	//	m_Enemies.push_back(std::unique_ptr<BasicMob>(new BasicMob(600, 400)));
	//}
}

void Level::update(float timeElapsed)
{
	m_QuadTree = std::unique_ptr<QuadTree>(new QuadTree(0, BoundingBox(m_Player.getSprite().getPosition().x - Window::Instance().getWidth() / 2 + 16.0f, m_Player.getSprite().getPosition().y - Window::Instance().getHeight() / 2 + 16.0f, Window::Instance().getWidth(), Window::Instance().getHeight())));
	
	for (auto it = m_Enemies.begin(); it != m_Enemies.end(); )
	{
		if ((*it)->shouldDestroy())
		{
			spawnItem((*it)->getSprite().getPosition());
			it = m_Enemies.erase(it);
		}
		else
		{
			(*it)->update(m_Terrain, timeElapsed);
			m_QuadTree->insert((*it).get());
			++it;
		}
	}

	for (auto it = m_Entities.begin(); it != m_Entities.end(); )
	{
		if ((*it)->shouldDestroy())
		{
			spawnItem((*it)->getSprite().getPosition());
			it = m_Entities.erase(it);
		}
		else
		{
			(*it)->update(m_Terrain, m_QuadTree, timeElapsed);
			++it;
		}
	}

	m_Terrain.update(timeElapsed);
	auto& objects = m_Terrain.getObjects();
	objects.push_back(m_PlayerPtr);
	std::sort(objects.begin(), objects.end(), [](const auto& a, const auto& b) { return a->getY() > b->getY(); });

}

void Level::render(Renderer& renderer)
{
	ResourceManager::getInstance().shader("basic_shader")->use();
	m_Terrain.render(renderer);

	for (auto& enemy : m_Enemies)
	{
		enemy->render(renderer);
	}

	auto& objects = m_Terrain.getObjects();

	renderer.begin();
	for (auto& entity : objects)
	{
		entity->submit(renderer);
	}
	renderer.end();
	renderer.flush();
	//renderer.render(m_Entities);

	ResourceManager::getInstance().shader("outline_shader")->use();
	m_PlayerPtr->render(renderer);
	ResourceManager::getInstance().shader("basic_shader")->use();

}

void Level::spawnItem(const glm::vec3 & position)
{
	int randomNum = Utils::random(0, 100);

	if (randomNum < 50)
	{
	}
}
