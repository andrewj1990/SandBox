#pragma once

#include "..\Entity.h"
#include "..\Particles\ParticleManager.h"
#include "..\Particles\Particle.h"

class Ragdoll : public Entity
{
public:
	Ragdoll(float x, float y, float size, float angleDeg, Texture* texture);
	~Ragdoll() {}

	void update(float timeElapsed) override;
	void submit(Renderer& renderer) override;

private:
	float m_KnockbackDistance;

};