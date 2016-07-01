#pragma once

#include "../Graphics/renderable.h"
#include <iostream>

class Sprite : public Renderable
{
public:
	Sprite();
	Sprite(const glm::vec3& position, const glm::vec2 size, const glm::vec4& colour);
	Sprite(const glm::vec3& position, const glm::vec2 size, Texture* texture);
	~Sprite();

	void setPosition(float x, float y);

	void fade(float fadeAmount = 0.01f);

	void setAngle(float angle) { m_Angle = angle; }
	float getAngle() const { return m_Angle; }

private:
	float m_Angle;

};