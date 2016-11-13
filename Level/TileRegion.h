#pragma once

#include <glm\glm.hpp>
#include <unordered_set>
#include "..\Graphics\Renderer.h"
#include "Tiles\Tile.h"
#include "..\Utils\simplexNoise.h"

class TileRegion {
public:
	TileRegion(int x, int y);
	TileRegion(int x, int y, int size);
	~TileRegion() { }

	void init(const std::unordered_set<std::string>& region_tiles);

	void render(Renderer& renderer);

	int indexX() { return m_IndexX; }
	int indexY() { return m_IndexY; }
	const std::vector<std::shared_ptr<Renderable>>& getTiles() { return m_Tiles; }

	void setUV(int x, int y, const std::unordered_set<std::string>& region_tiles);
	void removeTile(int x, int y);
	std::shared_ptr<Renderable>& getTile(int x, int y);
	std::vector<std::shared_ptr<Renderable>>::iterator getTileIterator(int x, int y);

private:
	void setTileUV(std::shared_ptr<Renderable>& tile, const std::unordered_set<std::string>& region_tiles);
	float noiseHeight(float x, float y);
	bool calculateTile(float x, float y, const std::unordered_set<std::string>& region_tiles);
	bool surfaceTile(float x, float y);
	bool transitionTile(float x, float y);
	bool caveTile(float x, float y, float threshold = 0.5f);

private:
	int m_IndexX;
	int m_IndexY;
	int m_X;
	int m_Y;
	int m_Size;
	int m_TileSize;

	float m_SurfaceTopEdge;
	float m_TransitionY;
	float m_NoiseSize;

	SimplexNoise m_Noise;
	std::vector<std::shared_ptr<Renderable>> m_Tiles;
};