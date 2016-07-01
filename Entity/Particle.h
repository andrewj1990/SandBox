#pragma once

#include "Entity.h"

class Particle : public Entity
{
public:
	Particle(float x, float y);
	
	void update(float timeElapsed) override;
	void render(Renderer& renderer) override;

private:
	float m_Duration;

};