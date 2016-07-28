#pragma once

#include "..\Entity.h"

class Bullet : public Entity
{
public:
	Bullet(float x, float y, float angle);

	virtual float getAngle() const override;

	void update(float timeElapsed);
	void render(Renderer& renderer);

private:
	float m_Duration;
};