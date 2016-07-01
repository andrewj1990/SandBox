#include "Level.h"

Level::Level()
	: m_Noise(), m_Player(Window::Instance().width() / 2 - 16.0f, Window::Instance().height() / 2 - 16.0f)
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

	//m_Enemies.push_back(std::unique_ptr<BasicMob>(new BasicMob(600, 400)));
	for (int i = 0; i < 21; i++)
	{
		m_Enemies.push_back(std::unique_ptr<BasicMob>(new BasicMob(600, 400)));
		//m_Enemies.push_back(std::unique_ptr<BasicMob>(new BasicMob(100, 100)));
	}
}

void Level::update(float timeElapsed)
{
	m_QuadTree = std::unique_ptr<QuadTree>(new QuadTree(0, BoundingBox(m_Player.getSprite().getPosition().x - Window::Instance().width() / 2 + 16.0f, m_Player.getSprite().getPosition().y - Window::Instance().height() / 2 + 16.0f, Window::Instance().width(), Window::Instance().height())));
	for (const auto& enemy : m_Enemies)
	{
		enemy->update(timeElapsed);
		m_QuadTree->insert(&enemy->getSprite());
	}

	m_Player.update(m_QuadTree, timeElapsed);
	m_Player.update(timeElapsed);

	std::vector<BoundingBox> bbox;
	m_QuadTree->getBounds(bbox);

	//std::cout << "bbox size : " << bbox.size() << "\n";

	float size = 2.0f;
	quads = std::vector<Renderable>();
	for (auto& box : bbox)
	{
		quads.push_back(Sprite(glm::vec3(box.x, box.y, 0.0f), glm::vec2(box.width, size), glm::vec4(1, 1, 0, 1)));
		quads.push_back(Sprite(glm::vec3(box.x, box.y + box.height, 0.0f), glm::vec2(box.width, size), glm::vec4(1, 1, 0, 1)));
		quads.push_back(Sprite(glm::vec3(box.x, box.y, 0.0f), glm::vec2(size, box.height), glm::vec4(1, 1, 0, 1)));
		quads.push_back(Sprite(glm::vec3(box.x + box.width, box.y, 0.0f), glm::vec2(size, box.height), glm::vec4(1, 1, 0, 1)));
	}
}

void Level::render(Renderer& renderer)
{
	for (const auto& enemy : m_Enemies)
	{
		enemy->render(renderer);
	}

	m_Player.render(renderer);

	//if (quads.size() > 0) renderer.render(quads);
}
