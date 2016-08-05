#include "Terrain.h"

Terrain::Terrain()
{
	m_TileSize = 8;
	m_NoiseSize = 1000.0f;
	init();
}

void Terrain::init()
{
	m_FireRadius = 100;

	m_QuadTree = std::unique_ptr<QuadTree>(new QuadTree(0, BoundingBox(Window::Instance().getCamera().getPosition().x, Window::Instance().getCamera().getPosition().x, Window::Instance().getWidth(), Window::Instance().getHeight())));

	m_Width = (Window::Instance().getWidth() / m_TileSize) + 2;
	m_Height = (Window::Instance().getHeight() / m_TileSize) + 3;

	for (int y = 0; y < m_Height; y++)
	{
		for (int x = 0; x < m_Width; x++)
		{
			float groundHeight = m_Noise.scaledOctaveNoise(5, 0.5, 1, 0, 1, x / m_NoiseSize, y / m_NoiseSize);
			m_Ground.push_back(getTile(x * m_TileSize, y * m_TileSize, groundHeight));
		}
	}
}

Renderable Terrain::getTile(float x, float y, float height)
{
	return Renderable(glm::vec3(x, y, 0), glm::vec2(m_TileSize, m_TileSize), getColor(height));
}

glm::vec4 Terrain::getColor(float height)
{
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	int solid = 0;

	if (height < 0.3)
	{
		b = 0.5f;
		solid = 1;
	}
	else if (height < 0.4)
	{
		b = 1.0f;
		solid = 1;
	}
	else if (height < 0.42)
	{
		r = 0.0f;
		g = height;
		b = 1.0f;
		solid = 1;
	}
	else if (height < 0.45)
	{
		r = 0.76f;
		g = 0.7f;
		b = 0.5f;
	}
	else if (height < 0.6)
	{
		r = 0.1f;
		g = height;
		b = 0.1f;
	}
	else if (height < 0.7)
	{
		g = height;
	}
	else
	{
		r = g = b = 1.0f;
	}

	return glm::vec4(r, g, b, solid);
}

bool Terrain::isSolid(float x, float y) const
{
	const glm::vec3& camPos = Window::Instance().getCamera().getPosition();

	// offset the position 1 tile forward to take into account the initial offset
	int xi = (x / m_TileSize) - (int)(camPos.x / m_TileSize) + 1;
	int yi = (y / m_TileSize) - (int)(camPos.y / m_TileSize) + 1;
	int index = xi + yi * m_Width;

	if (index < 0 || index >= m_Ground.size()) return true;

	const Renderable& tile = m_Ground[index];

	return tile.isSolid();
}

void Terrain::update(float timeElapsed)
{
	m_FireRadius += timeElapsed * 10;

	m_QuadTree = std::unique_ptr<QuadTree>(new QuadTree(0, BoundingBox(Window::Instance().getCamera().getPosition().x, Window::Instance().getCamera().getPosition().x, Window::Instance().getWidth(), Window::Instance().getHeight())));

	const glm::vec3& camPos = Window::Instance().getCamera().getPosition();

	// offset the position 1 tile back so we dont have any blank spaces
	int xp = (int)(camPos.x) / m_TileSize * m_TileSize - m_TileSize;
	int yp = (int)(camPos.y) / m_TileSize * m_TileSize - m_TileSize;

	for (int j = 0; j < m_Height; j++)
	{
		for (int i = 0; i < m_Width; i++)
		{
			float r = std::sqrtf((xp + i * m_TileSize) * (xp + i * m_TileSize) + (yp + j * m_TileSize) * (yp + j * m_TileSize));

			float groundHeight = m_Noise.scaledOctaveNoise(5, 0.5, 1, 0, 1, (xp / m_TileSize + i) / m_NoiseSize, (yp / m_TileSize + j) / m_NoiseSize);

			Renderable& terrain = m_Ground[i + j * m_Width];
			terrain.setPosition(glm::vec3(xp + i * m_TileSize, yp + j * m_TileSize, 0));

			glm::vec4 colour = r < m_FireRadius ? glm::vec4(0.2, 0.1, 0.1, 0) : getColor(groundHeight);
			terrain.setSolid(colour.w == 1);

			if (terrain.isSolid()) m_QuadTree->insert(&terrain);
			terrain.setColor(glm::vec3(colour));
		}
	}
}

void Terrain::render(Renderer& renderer)
{
	renderer.render(m_Ground);
}
