#include "TileRegion.h"

TileRegion::TileRegion(int x, int y)
	: m_IndexX(x), m_IndexY(y), m_Noise()
{
	m_Size = 16;
	m_X = x * (Settings::SUB_REGION_TILE_COUNT * Settings::TILE_SIZE);
	m_Y = y * (Settings::SUB_REGION_TILE_COUNT * Settings::TILE_SIZE);

	m_NoiseSize = 0.0f;
}

TileRegion::TileRegion(int x, int y, int size)
	: m_Size(size), m_IndexX(x), m_IndexY(y), m_Noise()
{
	m_X = x * (m_Size * m_Size);
	m_Y = y * (m_Size * m_Size);

	m_NoiseSize = 0.0f;
}

void TileRegion::init(const std::unordered_set<std::string>& region_tiles)
{
	m_NoiseSize = 2500.0f;
	m_SurfaceTopEdge = 1500.0f;
	m_TransitionY = -1000.0f;
	for (int x = 0; x < Settings::SUB_REGION_TILE_COUNT; x++)
	{
		for (int y = 0; y < Settings::SUB_REGION_TILE_COUNT; y++)
		{
			if (calculateTile(m_X + x * Settings::TILE_SIZE, m_Y + y * Settings::TILE_SIZE, region_tiles))
			{
				std::string positionInRegion = std::to_string(m_X + x * Settings::TILE_SIZE) + "_" + std::to_string(m_Y + y * Settings::TILE_SIZE);
				auto it = region_tiles.find(positionInRegion);
				if (it == region_tiles.end())
				{
					m_Tiles.push_back(std::unique_ptr<Tile>(new Tile(glm::vec2(Settings::TILE_SIZE, Settings::TILE_SIZE))));
					auto& t = m_Tiles.back();
					int di = Utils::random(0, 1);
					t->init(m_X + x * Settings::TILE_SIZE, m_Y + y * Settings::TILE_SIZE, glm::vec4(0, di, 0, 1), false, false);
				}
			}
		}
	}

	for (auto& tile : m_Tiles) 
	{
		setTileUV(tile, region_tiles);
	}
}

void TileRegion::submit(Renderer& renderer)
{
	for (const auto& tile : m_Tiles)
	{
		renderer.submit(*tile);
	}
}

void TileRegion::render(Renderer& renderer)
{
	renderer.render(m_Tiles);
}

void TileRegion::setUV(int x, int y, const std::unordered_set<std::string>& region_tiles)
{
	std::string pos = std::to_string(x) + "_" + std::to_string(y);
	auto it = region_tiles.find(pos);
	if (it != region_tiles.end()) return;

	auto tileIt = getTileIterator(x, y);
	if (tileIt == m_Tiles.end()) return;
	setTileUV(*tileIt, region_tiles);
}

void TileRegion::removeTile(int x, int y)
{
	auto tileIt = getTileIterator(x, y);
	if (tileIt == m_Tiles.end()) return;

	m_Tiles.erase(tileIt);

}

std::shared_ptr<Renderable>& TileRegion::getTile(int x, int y)
{
	int px = x - (m_IndexX * (Settings::SUB_REGION_TILE_COUNT - Settings::TILE_SIZE));
	int py = y - (m_IndexY * (Settings::SUB_REGION_TILE_COUNT - Settings::TILE_SIZE));

	for (auto& tile : m_Tiles)
	{
		if ((int)tile->getPosition().x == px && (int)tile->getPosition().y == py)
		{
			return tile;
		}
	}

	return m_Tiles[0];
}

std::vector<std::shared_ptr<Renderable>>::iterator TileRegion::getTileIterator(int x, int y)
{
	return std::find_if(m_Tiles.begin(), m_Tiles.end(), [x, y](const auto& tile) { return (tile->getPosition().x == x && tile->getPosition().y == y); });
}

void TileRegion::setTileUV(std::shared_ptr<Renderable>& tile, const std::unordered_set<std::string>& region_tiles)
{
	float rightTileX = tile->getPosition().x + Settings::TILE_SIZE;
	float leftTileX = tile->getPosition().x - Settings::TILE_SIZE;
	float aboveTileY = tile->getPosition().y + Settings::TILE_SIZE;
	float belowTileY = tile->getPosition().y - Settings::TILE_SIZE;

	bool above = calculateTile(tile->getPosition().x, aboveTileY, region_tiles);
	bool below = calculateTile(tile->getPosition().x, belowTileY, region_tiles);
	bool right = calculateTile(rightTileX, tile->getPosition().y, region_tiles);
	bool left = calculateTile(leftTileX, tile->getPosition().y, region_tiles);

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

bool TileRegion::calculateTile(float x, float y, const std::unordered_set<std::string>& region_tiles)
{
	auto it = region_tiles.find(std::to_string((int)x) + "_" + std::to_string((int)y));
	if (it != region_tiles.end())
	{
		return false;
	}

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
	float surface = m_Noise.scaledOctaveNoise(5, 0.5, 1, 0, 1, x / m_NoiseSize, y / m_NoiseSize) + grad;
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
