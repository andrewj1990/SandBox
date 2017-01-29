#pragma once

#include "TileRegion.h"
#include "..\Utils\QTree.h"
#include "..\Settings.h"
#include "TileRipple.h"

class Region
{
public:
	Region();
	~Region() {}

	void load(int x, int y);
	void unload(int x, int y);
	void addTiles(std::unique_ptr<QTree<Renderable>>& quadTree);
	void removeTiles(float x, float y, bool exactCoord = true, bool ripple = false);
	bool emptyTile(float x, float y);
	bool getSurfacePosition(float x, float y);

	void update(float timeElapsed);
	void render(Renderer& renderer);



private:
	void reloadTileUV(int x, int y);
	std::unique_ptr<TileRegion>& getTileRegion(int x, int y);

private:
	int m_X;
	int m_Y;
	int m_RegionWidth;
	int m_RegionHeight;
	int m_CountX;
	int m_CountY;

	int m_SubRegionWidth;
	int m_SubRegionHeight;

	std::vector<std::unique_ptr<TileRegion>> m_Regions;
	std::vector<std::unique_ptr<TileRipple>> m_Ripples;
	std::unordered_set<std::string> m_Tiles;
};