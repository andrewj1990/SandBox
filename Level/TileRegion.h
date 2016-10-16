#pragma once

#include <glm\glm.hpp>
#include "..\Graphics\Renderer.h"
#include "Tiles\Tile.h"

class TileRegion {
public:
	TileRegion(int x, int y);
	TileRegion(int x, int y, int size);
	~TileRegion() { }

	void init();

	void render(Renderer& renderer);

	int indexX() { return m_IndexX; }
	int indexY() { return m_IndexY; }

private:
	int m_IndexX;
	int m_IndexY;
	int m_X;
	int m_Y;
	int m_Size;

	std::vector<std::unique_ptr<Renderable>> m_Tiles;
};