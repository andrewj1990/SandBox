#pragma once

#include "..\Graphics\Sprite.h"

class Entity : public Sprite {
public:
	Entity();
	Entity(float x, float y);
	Entity(const glm::vec3& pos, const glm::vec2& size, Texture* texture);
	Entity(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& colour);
	~Entity();

	virtual void update(float timeElapsed) override;
	virtual void render(Renderer& renderer) override;

	virtual void setDestroy(bool flag) { m_Destroy = flag; }
	virtual bool shouldDestroy() const override { return m_Destroy; }

	float getX() const { return getPosition().x; }
	float getY() const { return getPosition().y; }
	float getCenterX() const { return getX() + getSize().x / 2.0f; }
	float getCenterY() const { return getY() + getSize().y / 2.0f; }

private:
	void init();

protected:
	float m_X;
	float m_Y;
	float m_Dx;
	float m_Dy;
	float m_Angle;

	bool m_Destroy;

	float m_CumulativeTime;

};