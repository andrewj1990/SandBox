#pragma once

#include "Mob.h"

class MeleeMob : public Mob
{
public:
	MeleeMob(float x, float y, std::unique_ptr<Player>& player);
	~MeleeMob();

	void attack(float x, float y) override;
	void damage(int amount) override;

	void update(float timeElapsed) override;
	void render(Renderer& renderer) override;

private:
	void attack(float timeElapsed);
	void move(float timeElapsed);
	void knockback(float timeElapsed);
	State determineState();

private:
	Sprite m_Spear;
	bool m_Attacking;
	float m_SpearAttack;
	float m_AttackSpeed;

	Timer m_AttackTime;
	float m_AttackDuration;

	Timer m_KnockbackTime;
	float m_KnockbackDuration;
};