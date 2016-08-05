#pragma once

#include "..\Graphics\Renderer.h"
#include "..\Utils\simplexNoise.h"
#include "..\Entity\player.h"
#include "..\Entity\Mob\BasicMob.h"
#include "..\Entity\Mob\Boss.h"
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
	const Player& getPlayer() const { return m_Player; }

private:
	void spawnItem(const glm::vec3& position);

private:
	Player m_Player;
	Terrain m_Terrain;

	Sprite treeSprite;

	std::shared_ptr<Player> m_PlayerPtr;

	std::vector<std::unique_ptr<Entity>> m_Enemies;
	std::vector<std::shared_ptr<Entity>> m_Entities;
	std::vector<Renderable> m_Platforms;

	SimplexNoise m_Noise;
	std::unique_ptr<QuadTree> m_QuadTree;

	std::vector<Renderable> quads;
};