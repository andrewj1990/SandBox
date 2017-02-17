#pragma once

#include "../Graphics/renderable.h"
#include <iostream>

class Sprite : public Renderable
{
public:
	Sprite();
	Sprite(const glm::vec3& position, const glm::vec2 size, const glm::vec4& colour = glm::vec4(1.0));
	Sprite(const glm::vec3& position, const glm::vec2 size, Texture* texture);
	virtual ~Sprite() {}

	virtual void init();

	void setPosition(float x, float y);
	void fade(float fadeAmount = 0.01f);
	void setAngle(float angle) { m_Angle = angle; }
	float getAngle() const { return m_Angle; }
	bool isSolid() const { return m_Solid; }

	void setDestroy(bool flag) { m_Destroy = flag; }
	bool shouldDestroy() const { return m_Destroy; }

	inline float getX() const { return getPosition().x; }
	inline float getY() const { return getPosition().y; }
	inline float getWidth() const { return getSize().x; }
	inline float getHeight() const { return getSize().y; }
	inline float getCenterX() const { return getX() + getSize().x / 2.0f; }
	inline float getCenterY() const { return getY() + getSize().y / 2.0f; }

	std::shared_ptr<BoundingBox>& getCollisionBox() { return m_CollisionBox; }

protected:
	float m_Angle;

	bool m_Solid;
	bool m_Destroy;

	std::shared_ptr<BoundingBox> m_CollisionBox;
};