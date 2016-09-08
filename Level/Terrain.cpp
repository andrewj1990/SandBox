#include "Terrain.h"

Terrain::Terrain()
{
	m_TileSize = 32;
	m_NoiseSize = 250.0f;
	init();
}

void Terrain::init()
{
	m_FireRadius = 100;
	m_WidthOffset = 4;
	m_HeightOffset = 4;

	m_Width = (Window::Instance().getWidth() / m_TileSize) + m_WidthOffset;
	m_Height = (Window::Instance().getHeight() / m_TileSize) + m_HeightOffset;

	for (int y = 0; y < m_Height; y++)
	{
		for (int x = 0; x < m_Width; x++)
		{
			m_Tiles.push_back(std::unique_ptr<Tile>(new Tile()));
		}
	}
}

void Terrain::addPlayerPtr()
{
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

glm::vec4 Terrain::getUV(float height)
{
	int x = 0;
	int y = 0;
	int solid = 0;

	if (height < 0.3)
	{
		x = 1;
		solid = 1;
	}
	else if (height < 0.4)
	{
		x = 1;
		solid = 1;
	}
	else if (height < 0.42)
	{
		x = 1;
		y = 1;
		solid = 1;
	}
	else if (height < 0.45)
	{
		x = 0;
		y = 1;
	}
	else if (height < 0.6)
	{
	}
	else if (height < 0.7)
	{
	}
	else
	{
	}

	return glm::vec4(x, y, solid, 0);
}

bool Terrain::isSolid(float x, float y) const
{
	const glm::vec3& camPos = Window::Instance().getCamera().getPosition();

	// offset the position 1 tile forward to take into account the initial offset
	int xi = (x / m_TileSize) - (int)(camPos.x / m_TileSize) + (m_WidthOffset / 2);
	int yi = (y / m_TileSize) - (int)(camPos.y / m_TileSize) + (m_HeightOffset / 2);
	int index = xi + yi * m_Width;

	if (index < 0 || index >= m_Tiles.size()) return true;

	const auto& tile = m_Tiles[index];

	return tile->isSolid();
}

void Terrain::update(float timeElapsed)
{
	m_FireRadius += timeElapsed * 10;

	const glm::vec3& camPos = Window::Instance().getCamera().getPosition();

	// offset the position 1 tile back so we dont have any blank spaces
	int xp = (int)(camPos.x) / m_TileSize * m_TileSize - (m_WidthOffset / 2) * m_TileSize;
	int yp = (int)(camPos.y) / m_TileSize * m_TileSize - (m_HeightOffset / 2) * m_TileSize;

	m_Objects = std::vector<std::shared_ptr<Entity>>();

	for (int j = 0; j < m_Height; j++)
	{
		for (int i = 0; i < m_Width; i++)
		{
			float r = std::sqrtf((xp + i * m_TileSize) * (xp + i * m_TileSize) + (yp + j * m_TileSize) * (yp + j * m_TileSize));

			float groundHeight = m_Noise.scaledOctaveNoise(5, 0.5, 1, 0, 1, (xp / m_TileSize + i) / m_NoiseSize, (yp / m_TileSize + j) / m_NoiseSize);

			glm::vec4 colour = r < m_FireRadius ? glm::vec4(0.2, 0.1, 0.1, 0) : getUV(groundHeight);
			bool isSolid = colour.w == 1;
			colour.w = 1.0f;
			std::unique_ptr<Renderable>& tile = m_Tiles[i + j * m_Width];

			float isTree = m_Noise.scaledOctaveNoise(1, 0.5, 1, 0, 1, (xp / m_TileSize + i), (yp / m_TileSize + j));
			tile->init(xp + i * m_TileSize, yp + j * m_TileSize, colour, isSolid, isTree > 0.90);
			
			if (tile->isTreeTile())
			{
				m_Objects.push_back(tile->getTree());
			}
		}
	}
}

void Terrain::render(Renderer& renderer)
{
	ResourceManager::getInstance().shader("basic_shader")->setUniform("outline", false);
	renderer.render(m_Tiles);
	ResourceManager::getInstance().shader("basic_shader")->setUniform("outline", true);

}
