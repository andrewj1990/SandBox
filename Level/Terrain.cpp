#include "Terrain.h"

Terrain::Terrain()
{
	init();
}

void Terrain::init()
{
	m_FireRadius = 100;

	m_Width = (Window::Instance().getWidth() / 32) + 2;
	m_Height = (Window::Instance().getHeight() / 32) + 3;

	for (int y = 0; y < m_Height; y++)
	{
		for (int x = 0; x < m_Width; x++)
		{
			float groundHeight = m_Noise.scaledOctaveNoise(5, 0.5, 1, 0, 1, x / 32.0f, y / 32.0f);
			m_Ground.push_back(getTile(x * 32, y * 32, groundHeight));
		}
	}
}

Renderable Terrain::getTile(float x, float y, float height)
{
	return Renderable(glm::vec3(x, y, 0), glm::vec2(32, 32), getColor(height));
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
		g = 0.5f;
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
		g = 0.8f;
		b = 0.1f;
	}
	else if (height < 0.7)
	{
		g = 0.6f;
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
	int xi = (x / 32) - (int)(camPos.x / 32) + 1;
	int yi = (y / 32) - (int)(camPos.y / 32) + 1;
	int index = xi + yi * m_Width;

	if (index < 0 || index >= m_Ground.size()) return true;

	const Renderable& tile = m_Ground[index];

	return tile.isSolid();
}

void Terrain::update(float timeElapsed)
{
	m_FireRadius += timeElapsed * 10;

	const glm::vec3& camPos = Window::Instance().getCamera().getPosition();

	// offset the position 1 tile back so we dont have any blank spaces
	int xp = (int)(camPos.x) / 32 * 32 - 32;
	int yp = (int)(camPos.y) / 32 * 32 - 32;

	for (int j = 0; j < m_Height; j++)
	{
		for (int i = 0; i < m_Width; i++)
		{
			float r = std::sqrtf((xp + i * 32) * (xp + i * 32) + (yp + j * 32) * (yp + j * 32));

			float groundHeight = m_Noise.scaledOctaveNoise(5, 0.5, 1, 0, 1, (xp / 32 + i) / 32.0f, (yp / 32 + j) / 32.0f);

			Renderable& terrain = m_Ground[i + j * m_Width];
			terrain.setPosition(glm::vec3(xp + i * 32, yp + j * 32, 0));

			glm::vec4 colour = r < m_FireRadius ? glm::vec4(0.2, 0.1, 0.1, 0) : getColor(groundHeight);
			terrain.setSolid(colour.w == 1);
			terrain.setColor(glm::vec3(colour));
		}
	}
}

void Terrain::render(Renderer & renderer)
{
	renderer.render(m_Ground);
}
