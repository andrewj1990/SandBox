#pragma once

#include "..\Entity.h"
#include "..\player.h"
#include "Actions\DamageAction.h"
#include "LifeBar.h"

class Mob : public Entity
{
public:
	Mob(const glm::vec3& position, const glm::vec2& size, Texture* texture, std::unique_ptr<Player>& player);
	~Mob() {}

	virtual void damage(float amount);
	virtual void attack(float x, float y) {}

	virtual void update(float timeElapsed);

	void playAction(float timeElapsed);

protected:
	float m_HP;
	float m_MaxHP;
	int m_ActionIndex;

	std::unique_ptr<Player>& m_Player;
	LifeBar m_LifeBar;

	std::vector<std::unique_ptr<Action>> m_Actions;

};