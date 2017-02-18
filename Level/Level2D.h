#pragma once

#include <unordered_set>
#include <algorithm>

#include "..\Graphics\Renderer.h"
#include "Tiles\Tile.h"
#include "Region.h"
#include "TileRegion.h"
#include "..\Entity\player.h"
#include "..\Graphics\Light.h"
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
	void moveCamera();
	
private:

	std::unique_ptr<Player> m_Player;

	Light m_Light;
	PointLight m_PointLight;
	std::unique_ptr<QTree<Sprite>> m_WaterTilesQT;
	std::unique_ptr<QTree<Sprite>> m_QuadTree;
	std::unique_ptr<QTree<Sprite>> m_ObjectsQT;
	Sprite m_Background;

	std::vector<Light> m_Lights;
	std::unordered_set<std::string> m_RegionSet;

	std::vector<std::unique_ptr<WaterRipple>> m_WaterRipples;

	Region m_Region;
	//bool m_ShowQuadTree;

	float m_WaterRippleTime;
	int m_Delay;
};