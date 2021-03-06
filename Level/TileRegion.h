#pragma once

#include <glm\glm.hpp>
#include <unordered_set>
#include "..\Graphics\Renderer.h"
#include "Tiles\Tile.h"
#include "..\Utils\simplexNoise.h"
#include "..\Settings.h"
#include "Objects\Tree.h"
#include "Objects\Rock.h"
#include "Objects\Bush.h"

class TileRegion {
public:
	TileRegion(int x, int y);
	TileRegion(int x, int y, int size);
	~TileRegion() { }

	void init(const std::unordered_set<std::string>& region_tiles, const std::unordered_set<std::string>& destroyed_objects);

	void submitTiles(Renderer& renderer);
	void submitObjects(Renderer& renderer);

	int indexX() { return m_IndexX; }
	int indexY() { return m_IndexY; }
	const std::vector<std::unique_ptr<Tile>>& getTiles() { return m_Tiles; }
	std::vector<std::unique_ptr<Entity>>& getObjects() { return m_Objects; }

	void setUV(int x, int y, const std::unordered_set<std::string>& region_tiles);
	void removeTile(int x, int y);
	std::unique_ptr<Tile>& getTile(int x, int y);
	TileType getTileType(int x, int y);
	TileType calculateTileType(int x, int y);
	std::vector<std::unique_ptr<Tile>>::iterator getTileIterator(int x, int y);
	bool calculateTile(float x, float y, const std::unordered_set<std::string>& region_tiles);
	bool surfaceTile(float x, float y);

private:
	void setTileUV(std::unique_ptr<Tile>& tile, const std::unordered_set<std::string>& region_tiles);
	float noiseHeight(float x, float y);
	bool transitionTile(float x, float y);
	bool caveTile(float x, float y, float threshold = 0.5f);

	void addObjects(int x, int y, const std::unordered_set<std::string>& destroyed_objects, TileType tileType);

private:
	int m_IndexX;
	int m_IndexY;
	int m_X;
	int m_Y;

	float m_SurfaceTopEdge;
	float m_TransitionY;
	float m_NoiseSize;

	SimplexNoise m_Noise;
	std::vector<std::unique_ptr<Tile>> m_Tiles;
	std::vector<std::unique_ptr<Entity>> m_Objects;
};