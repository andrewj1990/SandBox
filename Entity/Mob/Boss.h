#pragma once

#include "..\Entity.h"
#include "Actions\Action.h"
#include "Actions\MoveAction.h"
#include "..\player.h"

class Boss : public Entity
{
public:
	Boss(Player& player, float x, float y);

	void damage(int amount) override;

	void update(const Terrain& terrain, float timeElapsed) override;
	void update(float timeElapsed) override;
	void render(Renderer& renderer) override;

	void shoot(float angle) override;

private:
	float m_Life;

	int m_ActionIndex;
	std::vector<std::unique_ptr<Action>> m_Actions;

	std::vector<std::unique_ptr<Entity>> m_Bullets;

	Player& m_Player;
};