#pragma once

#include "..\Entity.h"

class CollisionParticle : public Entity
{
public:

	CollisionParticle(float x, float y, float angleDeg);
	CollisionParticle(float x, float y, float angleDeg, Texture* texture);
	virtual ~CollisionParticle() {}

	void init();

	virtual void update(float timeElapsed) override;
	virtual void submit(Renderer& renderer) override;

private:
	float m_Duration;
	float m_Dz;

	bool m_StartDestruction;
};