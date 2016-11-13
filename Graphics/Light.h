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
		: m_Position(glm::vec2(0, 0)), m_Direction(glm::vec2(0, 0)), m_Angle(0), m_Sprite(glm::vec3(0, 0, 0), glm::vec2(0, 0), glm::vec4(0, 1, 0, 1))
	{
	}

	Ray(glm::vec2 position, float angle)
		: m_Position(position), m_Direction(glm::vec2(0, 0)), m_Sprite(glm::vec3(position, 0), glm::vec2(0, 0), glm::vec4(0, 1, 0, 1))
	{
		m_Angle = glm::radians(angle);
		setDirection(m_Angle);
	}

	void setPosition(const glm::vec2& position)
	{
		m_Position = position;
		m_Sprite.setPosition(position.x, position.y);
	}

	void intersection(const std::vector<std::shared_ptr<Renderable>>& renderables, const BoundingBox& bbox)
	{
		glm::vec2 temp;
		float t1 = INT_MAX;
		float t2 = INT_MAX;
		
		for (auto renderable : renderables)
		{
			// calculate intersection on each line segment in the renderable
			int x = renderable->getPosition().x;
			int y = renderable->getPosition().y;
			int w = renderable->getSize().x;
			int h = renderable->getSize().y;

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

		// intersections of bounding box
		int x = bbox.x;
		int y = bbox.y;
		int w = bbox.width;
		int h = bbox.height;

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

		float ix = m_Position.x + m_Direction.x * t1;
		float iy = m_Position.y + m_Direction.y * t1;

		float dx = ix - m_Position.x;
		float dy = iy - m_Position.y;
		m_Length = std::sqrtf(dx * dx + dy * dy);
		m_Sprite.setSize(glm::vec2(m_Length, 2));

		m_EndPoint.x = ix;
		m_EndPoint.y = iy;
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

		//if (std::cosf(angle) == m_Direction.x && std::sinf(angle) == m_Direction.y) return glm::vec2(-1, -1);

		float t2 = (m_Direction.x * (sy - m_Position.y) + m_Direction.y * (m_Position.x - sx)) / (dx * m_Direction.y - dy * m_Direction.x);
		float t1 = (sx + dx * t2 - m_Position.x) / m_Direction.x;
		
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

	void setDirection(float angleR)
	{
		m_Direction.x = std::cosf(m_Angle);
		m_Direction.y = std::sinf(m_Angle);
	}

	void render(Renderer& renderer)
	{
		glm::mat4 m;
		m = glm::translate(m, glm::vec3(m_Sprite.getPosition().x, m_Sprite.getPosition().y, 0));
		m = glm::rotate(m, m_Angle, glm::vec3(0, 0, 1));
		m = glm::translate(m, glm::vec3(-m_Sprite.getPosition().x, -m_Sprite.getPosition().y, 0));
		renderer.push(m);
		renderer.render(m_Sprite);
		renderer.pop();
	}

	float getAngle() const { return m_Angle; }
	float getLength() const { return m_Length; }

	const glm::vec2& getPosition() const { return m_Position; }
	const glm::vec2& getEndPosition() const { return m_EndPoint; }

private:
	glm::vec2 m_Position;
	glm::vec2 m_EndPoint;
	glm::vec2 m_Direction;

	float m_Angle;
	float m_Length;

	Sprite m_Sprite;

};

class Light
{
public:
	Light();
	Light(const Light& other);
	~Light() {}

	void update(float x, float y, float timeElapsed);
	void update(const std::vector<std::shared_ptr<Renderable>> renderables, float timeElapsed);
	void render(Renderer& renderer);
	void renderShadow(Renderer& renderer);

	int getX() { return m_Point.getPosition().x; }
	int getY() { return m_Point.getPosition().y; }

	const BoundingBox& getLightRegion() { return m_LightRegion; }

private:
	//Renderable m_Point;
	Renderable m_Point;
	Sprite m_LightBox;
	BoundingBox m_LightRegion;

	Ray m_Ray;
	std::vector<Ray> m_Rays;
	std::vector<Renderable> m_RaySprites;

};