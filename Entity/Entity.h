#pragma once

#include "..\Graphics\Renderer.h"
#include "..\Graphics\Sprite.h"

class Entity {
public:
	Entity();
	Entity(float x, float y);
	~Entity();

	virtual void update(float timeElapsed);
	virtual void render(Renderer& renderer);

	virtual bool shouldDestroy() const { return m_Destroy; }
	virtual Sprite& getSprite() { return m_Sprite; }

protected:
	float m_X;
	float m_Y;
	float m_Dx;
	float m_Dy;

	bool m_Destroy;

	float m_CumulativeTime;

	Sprite m_Sprite;
};