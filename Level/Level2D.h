#pragma once

#include <unordered_set>
#include <algorithm>

#include "..\Graphics\Renderer.h"
#include "Tiles\Tile.h"
#include "Region.h"
#include "TileRegion.h"
#include "..\Entity\player.h"
#include "..\Graphics\PointLight.h"
#include "..\Utils\QTree.h"
#include "..\Graphics\Buffers\framebuffer.h"
#include "Effects\WaterRipple.h"

class Level2D 
{
public:
	Level2D();

	void init();

	void update(float timeElapsed);
	void render(Renderer& renderer);
	void renderLights(Renderer& renderer);

	const std::unique_ptr<Player>& getPlayerPtr() { return m_Player; }
	void waterRipple(int x, int y);
	
private:

	std::unique_ptr<Player> m_Player;

	PointLight m_PointLight;

	std::unique_ptr<QTree<Sprite>> m_WaterTilesQT;
	std::unique_ptr<QTree<Sprite>> m_QuadTree;
	std::unique_ptr<QTree<Sprite>> m_ObjectsQT;

	std::unique_ptr<QTree<Sprite>> m_TestQT;
	std::vector<std::shared_ptr<Sprite>> m_TestObjects;

	Sprite m_Background;

	std::vector<PointLight> m_PointLights;
	std::unordered_set<std::string> m_RegionSet;

	std::vector<std::unique_ptr<WaterRipple>> m_WaterRipples;

	Region m_Region;

	float m_WaterRippleTime;
};