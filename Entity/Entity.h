#pragma once

#include "..\Graphics\Sprite.h"

class Entity {
public:
	Entity();
	Entity(float x, float y);
	Entity(const glm::vec3& pos, const glm::vec2& size, Texture* texture);
	Entity(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& colour);
	~Entity();

	virtual void update(const Terrain& terrain, float timeElapsed) {}
	virtual void update(float timeElapsed);
	virtual void render(Renderer& renderer);

	virtual void setDestroy(bool flag) { m_Destroy = flag; }
	virtual bool shouldDestroy() const { return m_Destroy; }

	virtual float getAngle() const { return 0; }
	virtual void damage(int amount) {}

	float getX() const { return m_Sprite.getPosition().x; }
	float getY() const { return m_Sprite.getPosition().y; }
	float getCenterX() const { return getX() + m_Sprite.getSize().x / 2.0f; }
	float getCenterY() const { return getY() + m_Sprite.getSize().y / 2.0f; }

	Sprite& getSprite() { return m_Sprite; }

private:
	void init();

protected:
	Sprite m_Sprite;

	float m_X;
	float m_Y;
	float m_Dx;
	float m_Dy;
	float m_Angle;

	bool m_Destroy;

	float m_CumulativeTime;

};