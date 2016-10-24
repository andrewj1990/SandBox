#pragma once

// class made with the info from: http://ncase.me/sight-and-light/

#include "Renderer.h"
#include "renderable.h"
#include "..\Utils\quadTree.h"
#include "Sprite.h"
#include "..\Utils\QTree.h"

class Ray
{
public:
	Ray()
		: m_Position(glm::vec2(0, 0)), m_Direction(glm::vec2(0, 0)), m_Angle(0)
	{
	}

	Ray(glm::vec2 position, glm::vec2 direction)
		: m_Position(position), m_Direction(direction), m_Angle(0)
	{
	}

	void setPosition(const glm::vec2& position)
	{
		m_Position = position;
	}

	void intersection(const std::vector<Renderable*>& renderables)
	{
		glm::vec2 temp;
		float t1 = INT_MAX;
		float t2 = INT_MAX;

		//std::cout << "renderables size : " << renderables.size() << "\n";
		
		for (auto renderable : renderables)
		{
			// calculate intersection on each line segment in the renderable
			int x = renderable->getPosition().x;
			int y = renderable->getPosition().y;
			int w = renderable->getSize().x;
			int h = renderable->getSize().y;
			//std::cout << "pos : " << x << ", " << y << " | " << w << ", " << h << "\n";

			temp = intersection(x, y, x + w, y);
			if (intersection(temp.x, temp.y))
			{
				if (temp.x < t1)
				{
					t1 = temp.x;
					t2 = temp.y;
				}
			}

			temp = intersection(x, y, x, y + h);
			if (intersection(temp.x, temp.y))
			{
				if (temp.x < t1)
				{
					t1 = temp.x;
					t2 = temp.y;
				}
			}
			temp = intersection(x, y + h, x + w, y + h);
			if (intersection(temp.x, temp.y))
			{
				if (temp.x < t1)
				{
					t1 = temp.x;
					t2 = temp.y;
				}
			}
			temp = intersection(x + w, y, x + w, y + h);
			if (intersection(temp.x, temp.y))
			{
				if (temp.x < t1)
				{
					t1 = temp.x;
					t2 = temp.y;
				}
			}
		}

		if (intersection(t1, t2))
		{
			float ix = m_Position.x + m_Direction.x * t1;
			float iy = m_Position.y + m_Direction.y * t1;

			float dx = ix - m_Position.x;
			float dy = iy - m_Position.y;
			m_Length = std::sqrtf(dx * dx + dy * dy);
			//std::cout << "rayPos : " << m_Position.x << ", " << m_Position.y << " | " << ix << ", " << iy << "\n";
		}

	}

	bool intersection(float t1, float t2)
	{
		return (t1 > 0 && t2 > 0 && t2 < 1);
	}

	glm::vec2 intersection(float sx, float sy, float ex, float ey)
	{
		float dx = ex - sx;
		float dy = ey - sy;

		float angle = std::atan2f(dy, dx);

		//dx = std::cosf(angle);
		//dy = std::sinf(angle);

		if (dx == m_Direction.x && dy == m_Direction.y) return glm::vec2(-1, -1);

		//float r_dx = m_Direction.x;
		//float s_px = sx;
		//float s_py = sy;
		//float r_py = m_Position.y;
		//float r_dy = m_Direction.y;
		//float r_px = m_Position.x;
		//float s_dx = dx;
		//float s_dy = dy;

		//float t2 = (r_dx*(s_py - r_py) + r_dy*(r_px - s_px)) / (s_dx*r_dy - s_dy*r_dx);
		//float t1 = (s_px + s_dx*t2 - r_px) / r_dx;

		float t2 = (m_Direction.x * (sy - m_Position.y) + m_Direction.y * (m_Position.x - sx)) / (dx * m_Direction.y - dy * m_Direction.x);
		float t1 = (sx + dx * t2 - m_Position.x) / m_Direction.x;
		
		//std::cout << "sx : " << dx << ", " << dy << " | " << m_Direction.x << ", " << m_Direction.y << " | Angle : " << glm::degrees(m_Angle) << "\n";

		return glm::vec2(t1, t2);
	}

	// set direction based on a second point
	void calcAndSetDirection(float x, float y)
	{
		float dx = x - m_Position.x;
		float dy = y - m_Position.y;

		m_Angle = std::atan2f(dy, dx);

		m_Direction.x = std::cosf(m_Angle);
		m_Direction.y = std::sinf(m_Angle);
	}

	float getAngle() const { return m_Angle; }
	float getLength() const { return m_Length; }

private:
	glm::vec2 m_Position;
	glm::vec2 m_Direction;

	float m_Angle;
	float m_Length;

};

class Light
{
public:
	Light();
	~Light() {}

	void update(float timeElapsed);
	void update(const std::vector<Renderable*> renderables, float timeElapsed);
	void render(Renderer& renderer);

	int getX() { return m_Point.getPosition().x; }
	int getY() { return m_Point.getPosition().y; }

	const BoundingBox& getLightRegion() { return m_LightRegion; }

private:
	//Renderable m_Point;
	Renderable m_Point;
	Sprite m_LightBox;
	BoundingBox m_LightRegion;

	Ray m_Ray;

};