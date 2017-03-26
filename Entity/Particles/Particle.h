#pragma once

#include "..\..\Utils\Utils.h"
#include "..\Entity.h"

class Particle : public Entity
{
public:
	Particle(float x, float y, float size = 5, float angleDeg = 0);
	Particle(float x, float y, float size, Texture* texture);
	virtual ~Particle() {}
	
	virtual void update(float timeElapsed) override;

protected:
	float m_Duration;
	float m_RotationSpeed;
	float m_Dz;
};