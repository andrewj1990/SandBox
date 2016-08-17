#pragma once

#include "..\Entity.h"
#include "Actions\Action.h"
#include "Actions\MoveAction.h"
#include "..\player.h"
#include "..\..\Utils\astar.h"

class Boss : public Entity
{
public:
	Boss(std::shared_ptr<Player> player, float x, float y);

	void damage(int amount) override;

	void update(const Terrain& terrain, float timeElapsed) override;
	void update(float timeElapsed) override;
	void render(Renderer& renderer) override;

	void renderLight(Renderer& renderer) override;

	void shoot(float angle) override;

private:
	float m_Life;

	int m_ActionIndex;
	std::vector<std::unique_ptr<Action>> m_Actions;

	std::vector<std::unique_ptr<Entity>> m_Bullets;
	std::vector<std::unique_ptr<Entity>> m_Entities;

	std::shared_ptr<Player> m_Player;
	Sprite m_Light;
};