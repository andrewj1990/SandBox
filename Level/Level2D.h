#pragma once

#include "..\Graphics\Renderer.h"
#include "Tiles\Tile.h"
#include "TileRegion.h"

class Level2D 
{
public:
	Level2D();

	void load();

	void update(float timeElapsed);
	void render(Renderer& renderer);
	
private:
	void addTileRegion(int i, int j);
	void addTileRegionRow(int index, bool wholeRow = true);
	void addTileRegionCol(int index, bool wholeCol = true);
	void removeTileRegionRow(int index, bool wholeRowCol = true);
	void removeTileRegionCol(int index, bool wholeRowCol = true);

private:
	int m_RegionX;
	int m_RegionY;
	int m_RegionSize;

	std::vector<std::unique_ptr<Renderable>> m_Tiles;

	std::vector<std::unique_ptr<TileRegion>> m_TestRegion;


};