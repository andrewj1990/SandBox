#pragma once

#include "..\Entity.h"

class FireProjectile : public Entity
{
public:
	FireProjectile(float x, float y, float angleRad);

	void update(float timeElapsed);

	void submit(Renderer& renderer) override;
	void renderLight(Renderer& renderer) override;

private:
	float m_Duration;

	Sprite m_Light;
};