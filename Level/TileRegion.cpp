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
	for (int x = 0; x < m_Size; x++)
	{
		float height = m_Noise.scaledOctaveNoise(5, 0.5, 1, 0, 1, (m_X + x * m_TileSize) / m_NoiseSize, 0) * 1400;
		for (int y = 0; y < m_Size; y++)
		{
			//float height = m_Noise.scaledOctaveNoise(5, 0.5, 1, 0, 1, (m_X + x * m_TileSize) / m_NoiseSize, (m_Y + y * m_TileSize) / m_NoiseSize) * 1400;
			//float height = noiseHeight((m_X + x * m_TileSize) / m_NoiseSize, (m_Y + y * m_TileSize) / m_NoiseSize);
			float yy = m_Y + y * m_TileSize;
			//height += m_Noise.scaledOctaveNoise(5, 0.5, 1, 0, 1, (m_X + j * tile_size) / 250, 0) * 10;
			//std::cout << "height" << height << "\n";
			if (height > yy)
			{
				m_Tiles.push_back(std::unique_ptr<Tile>(new Tile(glm::vec2(m_TileSize, m_TileSize))));
				//auto& t = m_Tiles[j + i * m_Size];
				auto& t = m_Tiles.back();
				int di = Utils::random(0, 1);
				t->init(m_X + x * m_TileSize, m_Y + y * m_TileSize, glm::vec4(0, di, 0, 1), false, false);
				//if (di != 0) {
				//}
				//else
				//{
				//	t->init(m_X + j * tile_size, m_Y + i * tile_size, glm::vec4(1, 1, 0, 0), false, false);
				//}

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
	float height = m_Noise.scaledOctaveNoise(5, 0.5, 1, 0, 1, (tile->getPosition().x) / m_NoiseSize, 0) * 1400;
	//float height = noiseHeight((tile->getPosition().x) / m_NoiseSize, (tile->getPosition().y) / m_NoiseSize);

	float rightTileX = tile->getPosition().x + m_TileSize;
	float leftTileX = tile->getPosition().x - m_TileSize;
	float aboveTileY = tile->getPosition().y + m_TileSize;
	float belowTileY = tile->getPosition().y - m_TileSize;

	float rightHeight = m_Noise.scaledOctaveNoise(5, 0.5, 1, 0, 1, (rightTileX) / m_NoiseSize, 0) * 1400;
	float leftHeight = m_Noise.scaledOctaveNoise(5, 0.5, 1, 0, 1, (leftTileX) / m_NoiseSize, 0) * 1400;
	//float rightHeight = noiseHeight((rightTileX) / m_NoiseSize, (tile->getPosition().y) / m_NoiseSize);
	//float leftHeight = noiseHeight((leftTileX) / m_NoiseSize, (tile->getPosition().y) / m_NoiseSize);

	bool above = height > aboveTileY;
	bool below = height > belowTileY;
	bool right = rightHeight > tile->getPosition().y;
	bool left = leftHeight > tile->getPosition().y;
	bool topRight = rightHeight > aboveTileY;
	bool botRight = rightHeight > belowTileY;
	bool topLeft = leftHeight > aboveTileY;
	bool botLeft = leftHeight > belowTileY;

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

	//if (sum == 1) {
	//	tile->setUV(0, 0, 8, 8);
	//}
	//else if (sum == 5)
	//{
	//	tile->setUV(1, 1, 8, 8);
	//}
	//else if (sum == 4)
	//{
	//	tile->setUV(0, 0, 8, 8);
	//}
	//else
	//{
	//	tile->setUV(1, 1, 8, 8);
	//}
}

float TileRegion::noiseHeight(float x, float y)
{
	float height = m_Noise.scaledOctaveNoise(5, 0.5, 1, 0, 1, x, y) * 1400;
	return height;
}
