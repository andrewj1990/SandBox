#pragma once

#include "..\..\Entity\Particles\WaterParticle.h"
#include "..\Region.h"
#include <functional>

class WaterRipple
{
public:
	WaterRipple(float x, float y, float vel);

	bool complete();

	void update(Region& region, float timeElapsed);
	void submit(Renderer& renderer);

private:
	std::vector<std::unique_ptr<WaterParticle>> m_WaterParticles;

};