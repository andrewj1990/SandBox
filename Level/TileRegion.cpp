#include "TileRegion.h"

TileRegion::TileRegion(int x, int y)
	: m_IndexX(x), m_IndexY(y), m_Noise()
{
	m_Size = 16;
	m_X = x * (m_Size * m_Size);
	m_Y = y * (m_Size * m_Size);

	m_NoiseSize = 0.0f;
	m_TileSize = 0.0f;
}

TileRegion::TileRegion(int x, int y, int size)
	: m_Size(size), m_IndexX(x), m_IndexY(y), m_Noise()
{
	m_X = x * (m_Size * m_Size);
	m_Y = y * (m_Size * m_Size);

	m_NoiseSize = 0.0f;
	m_TileSize = 0.0f;
}

void TileRegion::init()
{
	m_TileSize = 16;
	m_NoiseSize = 2500.0f;
	m_SurfaceTopEdge = 1500.0f;
	m_TransitionY = -1000.0f;
	for (int x = 0; x < m_Size; x++)
	{
		for (int y = 0; y < m_Size; y++)
		{
			if (calculateTile(m_X + x * m_TileSize, m_Y + y * m_TileSize))
			{
					m_Tiles.push_back(std::unique_ptr<Tile>(new Tile(glm::vec2(m_TileSize, m_TileSize))));
					auto& t = m_Tiles.back();
					int di = Utils::random(0, 1);
					t->init(m_X + x * m_TileSize, m_Y + y * m_TileSize, glm::vec4(0, di, 0, 1), false, false);
			}
		}
	}

	for (auto& tile : m_Tiles) 
	{
		setTileUV(tile);
	}
}

void TileRegion::render(Renderer& renderer) 
{
	renderer.render(m_Tiles);
}

void TileRegion::setTileUV(std::unique_ptr<Renderable>& tile)
{
	float rightTileX = tile->getPosition().x + m_TileSize;
	float leftTileX = tile->getPosition().x - m_TileSize;
	float aboveTileY = tile->getPosition().y + m_TileSize;
	float belowTileY = tile->getPosition().y - m_TileSize;

	bool above = calculateTile(tile->getPosition().x, aboveTileY);
	bool below = calculateTile(tile->getPosition().x, belowTileY);
	bool right = calculateTile(rightTileX, tile->getPosition().y);
	bool left = calculateTile(leftTileX, tile->getPosition().y);

	int sum = 0;
	if (above) sum += 1;
	if (left)  sum += 2;
	if (below) sum += 4;
	if (right) sum += 8;
	//if (topRight) sum += 2;
	//if (botRight) sum += 8;
	//if (botLeft) sum += 32;
	//if (topLeft) sum += 128;

	tile->setUV(sum, 0, 16, 16);
}

float TileRegion::noiseHeight(float x, float y)
{
	float height = m_Noise.scaledOctaveNoise(5, 0.5, 1, 0, 1, x, y) * 1400;
	return height;
}

bool TileRegion::calculateTile(float x, float y)
{
	if (y > 0) 
	{
		return surfaceTile(x, y);
	}
	else if (y > m_TransitionY)
	{
		return transitionTile(x, y);
	}
	else
	{
		return caveTile(x, y);
	}

	return false;
}

bool TileRegion::surfaceTile(float x, float y)
{
	float grad = (m_SurfaceTopEdge - y) / m_SurfaceTopEdge;
	float surface = m_Noise.scaledOctaveNoise(5, 0.5, 1, 0, 1, x / m_NoiseSize, y / m_NoiseSize) +grad;
	return surface > 1.0f;
}

bool TileRegion::transitionTile(float x, float y)
{
	float caveEdge = -1000.0f;
	float grad = (caveEdge - y) / caveEdge;
	float surface = m_Noise.scaledOctaveNoise(5, 0.5, 1, 0, 1, x / m_NoiseSize, y / m_NoiseSize) + grad;
	return surface > 0.3f;
}

bool TileRegion::caveTile(float x, float y, float threshold)
{
	float noise = m_Noise.scaledOctaveNoise(5, 0.5, 1, 0, 1, x / m_NoiseSize, y / m_NoiseSize);
	return noise > threshold || noise < 0.35f;
}
