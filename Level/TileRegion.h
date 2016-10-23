#pragma once

#include <glm\glm.hpp>
#include "..\Graphics\Renderer.h"
#include "Tiles\Tile.h"
#include "..\Utils\simplexNoise.h"

class TileRegion {
public:
	TileRegion(int x, int y);
	TileRegion(int x, int y, int size);
	~TileRegion() { }

	void init();

	void render(Renderer& renderer);

	int indexX() { return m_IndexX; }
	int indexY() { return m_IndexY; }
	const std::vector<std::unique_ptr<Renderable>>& getTiles() { return m_Tiles; }

private:
	void setTileUV(std::unique_ptr<Renderable>& tile);
	float noiseHeight(float x, float y);
	bool calculateTile(float x, float y);
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
	std::vector<std::unique_ptr<Renderable>> m_Tiles;
};