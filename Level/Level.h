#pragma once

#include "..\Graphics\Renderer.h"
#include "..\Utils\simplexNoise.h"
#include "..\Entity\player.h"
#include "..\Entity\Mob\BasicMob.h"
#include "..\Utils\quadTree.h"
#include "Terrain.h"
#include "..\Utils\Utils.h"

class Level
{
public:
	Level();

	void init();

	void update(float timeElapsed);
	void render(Renderer& renderer);

	const std::unique_ptr<QuadTree>& getQuadTree() const { return m_QuadTree; }

private:
	void spawnItem(const glm::vec3& position);

private:
	Player m_Player;
	Terrain m_Terrain;

	std::vector<std::unique_ptr<Renderable>> m_Enemies;
	std::vector<Renderable> m_Platforms;

	SimplexNoise m_Noise;
	std::unique_ptr<QuadTree> m_QuadTree;

	std::vector<Renderable> quads;
};