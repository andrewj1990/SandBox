#pragma once

#include "TileRegion.h"
#include "..\Utils\QTree.h"
#include "..\Settings.h"
#include "Effects\TileRipple.h"

struct WorldObjects
{
	bool tree = false;
};

class Region
{
public:
	Region();
	~Region() {}

	void load(int x, int y);
	void unload(int x, int y);

	void load();
	void unload();
	
	void addTiles(std::shared_ptr<QTree<Sprite>>& quadTree);
	void addWaterTiles(std::shared_ptr<QTree<Sprite>>& quadTree);
	void addObjects(std::shared_ptr<QTree<Entity>>& quadTree);

	void removeTiles(float x, float y, bool exactCoord = true, bool ripple = false);
	void removeObject(float x, float y, bool exactCoord = true);
	TileType getTileType(float x, float y);
	bool emptyTile(float x, float y);
	bool getSurfacePosition(float x, float y);

	void update(float timeElapsed);
	void render(Renderer& renderer);

private:
	bool contains(int x, int y);
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

	BoundingBox m_UnloadRegion;
	BoundingBox m_LoadRegion;

	std::vector<std::unique_ptr<TileRegion>> m_Regions;
	std::vector<std::unique_ptr<TileRipple>> m_Ripples;
	std::unordered_set<std::string> m_Tiles;
	std::unordered_set<std::string> m_DestroyedObjects;

	std::unordered_map<std::string, WorldObjects> m_WorldObjects;
};