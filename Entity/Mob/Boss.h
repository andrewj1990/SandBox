#pragma once

#include "..\Entity.h"
#include "Actions\Action.h"
#include "Actions\MoveAction.h"

class Boss : public Entity
{
public:
	Boss(float x, float y);

	void damage(int amount) override;

	void update(const Terrain& terrain, float timeElapsed) override;
	void update(float timeElapsed) override;
	void render(Renderer& renderer) override;

private:
	float m_Life;

	int m_ActionIndex;
	std::vector<std::unique_ptr<Action>> m_Actions;

};