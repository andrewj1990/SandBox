#include "Ray.h"

Ray::Ray(glm::vec2 startPoint, float angleRad)
	: m_StartPoint(startPoint), m_EndPoint(0, 0), m_AngleRad(angleRad), m_T1(INT_MAX)
{
	m_Direction.x = std::cosf(m_AngleRad);
	m_Direction.y = std::sinf(m_AngleRad);
}

void Ray::findIntersections(const std::vector<std::shared_ptr<Sprite>>& sprites, const BoundingBox& bbox, float radius)
{
	m_T1 = INT_MAX;
	for (const auto& sprite : sprites)
	{
		// calculate intersection on each line segment in the renderable
		int x = sprite->getCollisionBox()->x;
		int y = sprite->getCollisionBox()->y;
		int w = sprite->getCollisionBox()->width;
		int h = sprite->getCollisionBox()->height;

		if (!Utils::inRange(m_StartPoint.x, m_StartPoint.y, x, y, radius)) continue;

		// find the end point
		intersect(x    , y	  , x + w, y    );
		intersect(x    , y    , x    , y + h);
		intersect(x    , y + h, x + w, y + h);
		intersect(x + w, y    , x + w, y + h);
	}
	
	// intersections of bounding box
	int x = bbox.x;
	int y = bbox.y;
	int w = bbox.width;
	int h = bbox.height;

	// find the end point
	intersect(x    , y    , x + w, y    );
	intersect(x    , y    , x    , y + h);
	intersect(x    , y + h, x + w, y + h);
	intersect(x + w, y    , x + w, y + h);
}

void Ray::intersect(float sx, float sy, float ex, float ey)
{
	float dx = ex - sx;
	float dy = ey - sy;
	
	float t2 = (m_Direction.x * (sy - m_StartPoint.y) + m_Direction.y * (m_StartPoint.x - sx)) / (dx * m_Direction.y - dy * m_Direction.x);
	float t1 = (sx + dx * t2 - m_StartPoint.x) / m_Direction.x;

	
	if (t1 > 0 && t2 > 0 && t2 < 1)
	{
		if (t1 < m_T1)
		{
			m_T1 = t1;
			m_EndPoint.x = m_StartPoint.x + m_Direction.x * t1;
			m_EndPoint.y = m_StartPoint.y + m_Direction.y * t1;
		}
	}
}

