#pragma once

#include "Particle.h"

class GunParticle : public Particle
{
public:
	GunParticle(float x, float y, float angle, float movespeed = 0.0f, float maxAngle = 90.0f, float minSpeed = 50.0f, float maxSpeed = 400.0f);

private:
};