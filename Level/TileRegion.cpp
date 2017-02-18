#include "TileRegion.h"

TileRegion::TileRegion(int x, int y)
	: m_IndexX(x), m_IndexY(y), m_Noise()
{
	m_X = x * (Settings::Instance().SUB_REGION_TILE_COUNT * Settings::Instance().TILE_SIZE);
	m_Y = y * (Settings::Instance().SUB_REGION_TILE_COUNT * Settings::Instance().TILE_SIZE);

	m_NoiseSize = 0.0f;
}

TileRegion::TileRegion(int x, int y, int size)
	: m_IndexX(x), m_IndexY(y), m_Noise()
{
	m_X = x * (size * size);
	m_Y = y * (size * size);

	m_NoiseSize = 0.0f;
}

void TileRegion::init(const std::unordered_set<std::string>& region_tiles)
{
	m_NoiseSize = 2500.0f;
	//m_SurfaceTopEdge = 1500.0f;
	//m_TransitionY = -1000.0f;
	//for (int x = 0; x < Settings::Instance().SUB_REGION_TILE_COUNT; x++)
	//{
	//	for (int y = 0; y < Settings::Instance().SUB_REGION_TILE_COUNT; y++)
	//	{
	//		if (calculateTile(m_X + x * Settings::Instance().TILE_SIZE, m_Y + y * Settings::Instance().TILE_SIZE, region_tiles))
	//		{
	//			std::string positionInRegion = std::to_string(m_X + x * Settings::Instance().TILE_SIZE) + "_" + std::to_string(m_Y + y * Settings::Instance().TILE_SIZE);
	//			auto it = region_tiles.find(positionInRegion);
	//			if (it == region_tiles.end())
	//			{
	//				m_Tiles.push_back(std::unique_ptr<Tile>(new Tile(glm::vec2(Settings::Instance().TILE_SIZE, Settings::Instance().TILE_SIZE))));
	//				auto& t = m_Tiles.back();
	//				int di = Utils::random(0, 1);
	//				t->init(m_X + x * Settings::Instance().TILE_SIZE, m_Y + y * Settings::Instance().TILE_SIZE, glm::vec4(0, di, 0, 1), false, false);
	//			}
	//		}
	//	}
	//}

	for (int x = 0; x < Settings::Instance().SUB_REGION_TILE_COUNT; x++)
	{
		for (int y = 0; y < Settings::Instance().SUB_REGION_TILE_COUNT; y++)
		{
			std::string positionInRegion = std::to_string(m_X + x * Settings::Instance().TILE_SIZE) + "_" + std::to_string(m_Y + y * Settings::Instance().TILE_SIZE);
			auto it = region_tiles.find(positionInRegion);
			if (it == region_tiles.end())
			{
				m_Tiles.push_back(std::unique_ptr<Tile>(new Tile(glm::vec2(Settings::Instance().TILE_SIZE, Settings::Instance().TILE_SIZE))));
				auto& t = m_Tiles.back();
				int di = Utils::random(0, 1);
				t->init(m_X + x * Settings::Instance().TILE_SIZE, m_Y + y * Settings::Instance().TILE_SIZE, glm::vec4(0, di, 0, 1), false);

				auto tileType = calculateTileType(m_X + x * Settings::Instance().TILE_SIZE, m_Y + y * Settings::Instance().TILE_SIZE);
				t->setUV(0, tileType, 16, 16);
				t->setType(tileType);

				// add objects to tile
				if (tileType == TileType::GRASS && Utils::chance(5))
				{
					m_Objects.push_back(std::shared_ptr<Sprite>(new Tree(m_X + x * Settings::Instance().TILE_SIZE, m_Y + y * Settings::Instance().TILE_SIZE)));
					//m_Objects.push_back(std::shared_ptr<Sprite>(new Sprite(glm::vec3(m_X + x * Settings::Instance().TILE_SIZE, m_Y + y * Settings::Instance().TILE_SIZE, -(m_Y + y * Settings::Instance().TILE_SIZE)), glm::vec2(32, 32), TextureManager::get("Textures/Tree.png"))));
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

	for (const auto& object : m_Objects)
	{
		object->submit(renderer);
	}
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

	//std::cout << "tile type : " << (*tileIt)->getType() << "\n";
	//(*tileIt)->setType(TileType::VOID);

	m_Tiles.erase(tileIt);
}

std::shared_ptr<Tile>& TileRegion::getTile(int x, int y)
{
	int px = x - (m_IndexX * (Settings::Instance().SUB_REGION_TILE_COUNT - Settings::Instance().TILE_SIZE));
	int py = y - (m_IndexY * (Settings::Instance().SUB_REGION_TILE_COUNT - Settings::Instance().TILE_SIZE));

	for (auto& tile : m_Tiles)
	{
		if ((int)tile->getPosition().x == px && (int)tile->getPosition().y == py)
		{
			return tile;
		}	
	}

	return m_Tiles[0];
}

TileType TileRegion::getTileType(int x, int y)
{
	auto tileIt = getTileIterator(x, y);
	if (tileIt == m_Tiles.end()) return TileType::VOID;

	return (*tileIt)->getType();
}

TileType TileRegion::calculateTileType(int x, int y)
{
	float elavation = m_Noise.scaledOctaveNoise(5, 0.5, 1, 0, 1, (float)(x) / m_NoiseSize, (float)(y) / m_NoiseSize);
	if (elavation < 0.2)
	{
		return TileType::DEEP_WATER;
	}
	else if (elavation < 0.4)
	{
		return TileType::SHALLOW_WATER;
	}
	else if (elavation < 0.5)
	{
		return TileType::SAND;
	}
	else
	{
		return TileType::GRASS;
	}

	return TileType::VOID;
}

std::vector<std::shared_ptr<Tile>>::iterator TileRegion::getTileIterator(int x, int y)
{
	return std::find_if(m_Tiles.begin(), m_Tiles.end(), [x, y](const auto& tile) { return (tile->getPosition().x == x && tile->getPosition().y == y); });
}

void TileRegion::setTileUV(std::shared_ptr<Tile>& tile, const std::unordered_set<std::string>& region_tiles)
{
	float sum = 0;

	float rightTileX = tile->getX() + Settings::Instance().TILE_SIZE;
	float leftTileX = tile->getX() - Settings::Instance().TILE_SIZE;
	float aboveTileY = tile->getY() + Settings::Instance().TILE_SIZE;
	float bottomTileY = tile->getY() - Settings::Instance().TILE_SIZE;

	auto aboveTileType = calculateTile(tile->getX(), aboveTileY, region_tiles);
	auto rightTileType = calculateTile(rightTileX, tile->getY(), region_tiles);
	auto bottomTileType = calculateTile(tile->getX(), bottomTileY,	 region_tiles);
	auto leftTileType = calculateTile(leftTileX, tile->getY(), region_tiles);

	if (aboveTileType) sum += 1.0f;
	if (rightTileType)  sum += 2.0f;
	if (bottomTileType) sum += 4.0f;
	if (leftTileType) sum += 8.0f;

	tile->setUV(sum, tile->getType(), 16, 16);
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
		return true;
	}

	auto tileType = calculateTileType(x, y);
	if (tileType == TileType::VOID) return true;

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
