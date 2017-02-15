#pragma once

#include "Particle.h"
#include "..\..\Level\Region.h"

class WaterParticle : public Particle
{
public:
	WaterParticle(float x, float y, float angleDeg, float vel = 0.0f);

	void update(Region& region, float timeElapsed);

private:
};