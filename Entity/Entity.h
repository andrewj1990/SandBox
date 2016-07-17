#pragma once

#include "..\Graphics\Sprite.h"

class Entity {
public:
	Entity();
	Entity(float x, float y);
	~Entity();

	virtual void update(float timeElapsed);
	virtual void render(Renderer& renderer);

	void setDestroy(bool flag) { m_Destroy = flag; }
	bool shouldDestroy() const { return m_Destroy; }
	Sprite& getSprite() { return m_Sprite; }

	float getX() const { return m_Sprite.getPosition().x; }
	float getY() const { return m_Sprite.getPosition().y; }
	float getCenterX() const { return getX() + m_Sprite.getSize().x / 2.0f; }
	float getCenterY() const { return getY() + m_Sprite.getSize().y / 2.0f; }

protected:
	float m_X;
	float m_Y;
	float m_Dx;
	float m_Dy;

	bool m_Destroy;

	float m_CumulativeTime;

	Sprite m_Sprite;
};