#pragma once

#include <unordered_set>
#include <algorithm>

#include "..\Graphics\Renderer.h"
#include "Tiles\Tile.h"
#include "Region.h"
#include "TileRegion.h"
#include "..\Entity\player.h"
#include "..\Graphics\Light.h"
#include "..\Utils\QTree.h"
#include "..\Graphics\Buffers\framebuffer.h"

class Level2D 
{
public:
	Level2D();

	void init();

	void update(float timeElapsed);
	void render(Renderer& renderer);
	void renderLights(Renderer& renderer);

	const std::unique_ptr<Player>& getPlayerPtr() { return m_Player; }

private:
	void moveCamera();
	
private:
	std::unique_ptr<Player> m_Player;

	Light m_Light;
	std::unique_ptr<QTree<Renderable>> m_QTree;
	std::unique_ptr<QTree<Renderable>> m_QuadTree;
	Sprite m_Background;

	std::vector<Light> m_Lights;
	std::unordered_set<std::string> m_RegionSet;

	Region m_Region;
	//bool m_ShowQuadTree;

	int m_Delay;
};