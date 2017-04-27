#pragma once

#include "..\..\Utils\Utils.h"
#include "..\Entity.h"

class Particle : public Entity
{
public:
	Particle(float x, float y, float size = 5, float angleDeg = 0);
	Particle(float x, float y, float size, const glm::vec4& colour, float angleDeg);
	Particle(float x, float y, float size, Texture* texture);
	Particle(float x, float y, float size, float angleDeg, Texture* texture);
	virtual ~Particle() {}
	
	virtual void update(float timeElapsed) override;
	virtual void submit(Renderer& renderer) override;

protected:
	float m_Duration;
	float m_RotationSpeed;
	float m_Dz;
};