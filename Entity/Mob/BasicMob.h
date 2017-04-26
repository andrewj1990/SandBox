#pragma once

#include "Mob.h"
#include "Actions\DamageAction.h"

class BasicMob : public Mob
{
public:
	BasicMob(float x, float y, std::unique_ptr<Player>& player);
	~BasicMob();

	void attack(float x, float y) override;
	void damage(int amount) override;

	void update(float timeElapsed) override;
	void render(Renderer& renderer) override;

private:
	std::vector<std::unique_ptr<Sprite>> m_FireRings;

	Sprite m_Spear;
	bool m_Attacking;
	float m_SpearAttack;

};