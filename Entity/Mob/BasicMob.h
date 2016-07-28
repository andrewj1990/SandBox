#pragma once

#include "..\Entity.h"

class BasicMob : public Entity
{
public:
	BasicMob(float x, float y);
	~BasicMob();

	void damage(int amount) override;

	void update(const Terrain& terrain, float timeElapsed);
	void update(float timeElapsed) override;
	void render(Renderer& renderer) override;

private:
	int m_Life;

};