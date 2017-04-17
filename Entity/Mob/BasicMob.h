#pragma once

#include "Mob.h"

class BasicMob : public Mob
{
public:
	BasicMob(float x, float y, std::unique_ptr<Player>& player);
	~BasicMob();

	void damage(int amount) override;

	void update(float timeElapsed) override;
	void render(Renderer& renderer) override;

private:

};