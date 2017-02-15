#pragma once

#include "..\Entity.h"

class Bullet : public Entity
{
public:
	Bullet(float x, float y, float angle);

	void update(float timeElapsed);
	void render(Renderer& renderer);

	void renderLight(Renderer& renderer) override;

private:
	float m_Duration;

	Sprite m_Light;
};