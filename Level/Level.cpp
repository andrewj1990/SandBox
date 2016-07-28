#include "Level.h"

Level::Level()
	: m_Player(Window::Instance().getWidth() / 2 - 16.0f, Window::Instance().getHeight() / 2 - 16.0f)
{
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

	m_Enemies.push_back(std::unique_ptr<BasicMob>(new BasicMob(600, 400)));
	for (int i = 0; i < 11; i++)
	{
		m_Enemies.push_back(std::unique_ptr<BasicMob>(new BasicMob(600, 400)));
	}
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

	if (m_Enemies.size() < 1000)
	{
		float wx = Window::Instance().getCamera().getPosition().x;
		float wy = Window::Instance().getCamera().getPosition().y;
		float ww = Window::Instance().getWidth();
		float wh = Window::Instance().getHeight();

		float sx = Utils::random(wx, wx + ww);
		float sy = Utils::random(wy, wy + wh);
		if (!m_Terrain.isSolid(sx, sy))
		{
			m_Enemies.push_back(std::unique_ptr<BasicMob>(new BasicMob(sx, sy)));
		}
	}

	m_Player.update(m_Terrain, m_QuadTree, timeElapsed);

	m_Terrain.update(timeElapsed);
}

void Level::render(Renderer& renderer)
{
	m_Terrain.render(renderer);

	renderer.render(m_Enemies);

	m_Player.render(renderer);
}

void Level::spawnItem(const glm::vec3 & position)
{
	int randomNum = Utils::random(0, 100);

	if (randomNum < 50)
	{
	}
}
