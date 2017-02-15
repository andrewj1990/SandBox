#pragma once

#include "..\..\Entity\Particles\WaterParticle.h"
#include "..\Region.h"

class WaterRipple
{
public:
	WaterRipple(float x, float y, float vel);

	bool complete();

	void update(Region& region, float timeElapsed);
	void render(Renderer& renderer);

private:
	std::vector<std::unique_ptr<WaterParticle>> m_WaterParticles;

};