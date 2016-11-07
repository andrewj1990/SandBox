#pragma once

#include <unordered_set>
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
	
private:
	std::unique_ptr<Player> m_Player;

	Light m_Light;
	std::unique_ptr<QTree<Renderable>> m_QTree;
	Sprite m_Background;

	std::vector<Light> m_Lights;
	std::unordered_set<std::string> m_RegionSet;

	Region m_Region;

	int m_Delay;
};