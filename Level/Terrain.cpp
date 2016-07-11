#include "Terrain.h"

Terrain::Terrain()
{
	init();
}

void Terrain::init()
{
	int width = Window::Instance().width() / 32;
	int height = Window::Instance().height() / 32;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			float groundHeight = m_Noise.scaledOctaveNoise(5, 0.5, 1, 0, 1, x / 32.0f, y / 32.0f);
			m_Ground.push_back(getTile(x * 32, y * 32, groundHeight));
		}
	}
}

Renderable Terrain::getTile(float x, float y, float height)
{
	return Renderable(glm::vec3(x, y, 0), glm::vec2(32, 32), glm::vec4(getColor(height), 1));
}

glm::vec3 Terrain::getColor(float height)
{
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;

	if (height < 0.3)
	{
		b = 0.5f;
	}
	else if (height < 0.4)
	{
		b = 1.0f;
	}
	else if (height < 0.42)
	{
		r = 0.0f;
		b = 1.0f;
		g = 0.5f;
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

	return glm::vec3(r, g, b);
}

void Terrain::update(float x, float y, float timeElapsed)
{
	int width = Window::Instance().width() / 32;
	int height = Window::Instance().height() / 32;

	int xp = (int)x / 32 * 32;
	int yp = (int)y / 32 * 32;

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			float groundHeight = m_Noise.scaledOctaveNoise(5, 0.5, 1, 0, 1, (xp / 32 + i) / 32.0f, (yp / 32 + j) / 32.0f);

			Renderable& terrain = m_Ground[i + j * width];
			terrain.setPosition(glm::vec3(xp + i * 32, yp + j * 32, 0));
			terrain.setColor(getColor(groundHeight));
		}
	}
}

void Terrain::render(Renderer & renderer)
{
	renderer.render(m_Ground);
}
