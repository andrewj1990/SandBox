#pragma once
// class made with the info from: http://ncase.me/sight-and-light/

#include "..\Graphics\Sprite.h"
#include "BoundingBox.h"

class Ray
{
public:
	Ray(glm::vec2 startPoint, float angleRad);

	void findIntersections(const std::vector<std::shared_ptr<Sprite>>& sprites, const BoundingBox& bbox, float radius);
	void intersect(float sx, float sy, float ex, float ey);

	const glm::vec2& getEndPoint() const { return m_EndPoint; }
	float getAngle() const { return m_AngleRad; }

private:
	glm::vec2 m_StartPoint;
	glm::vec2 m_EndPoint;
	glm::vec2 m_Direction;

	float m_AngleRad;
	float m_T1;
};
