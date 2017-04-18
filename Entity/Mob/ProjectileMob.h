#pragma once

#include "Mob.h"
#include "..\Projectile\FireProjectile.h"

class ProjectileMob : public Mob
{
public:
	ProjectileMob(float x, float y, std::unique_ptr<Player>& player);
	~ProjectileMob();

	void attack(float x, float y) override;
	void damage(int amount) override;

	void update(float timeElapsed) override;
	void render(Renderer& renderer) override;

private:
	std::vector<std::unique_ptr<Entity>> m_Projectiles;

};