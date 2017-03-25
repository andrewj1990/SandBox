#pragma once

#include "Component.h"
#include "..\..\Graphics\texture.h"
#include "..\..\Graphics\Sprite.h"

struct PositionComponent : public Component
{
	PositionComponent(const glm::vec3& position) 
		: Component(), m_Position(position)
	{
		m_ID = TypeID::value<PositionComponent>();
	}

	~PositionComponent() {}
	
	int m_X;
	int m_Y;

	glm::vec3 m_Position;
};

struct VelocityComponent : public Component
{
	VelocityComponent(int x, int y)
		: Component(), m_X(x), m_Y(y)
	{
		m_ID = TypeID::value<VelocityComponent>();
	}

	~VelocityComponent() {}

	int m_X;
	int m_Y;
};

struct InputComponent : public Component
{
	InputComponent()
		: Component()
	{
		m_ID = TypeID::value<InputComponent>();
	}

	~InputComponent() {}
};

struct SpriteComponent : public Component
{
	SpriteComponent(const glm::vec2& size, Texture* texture)
		: Component(), m_Size(size), m_Texture(texture)
	{
		m_ID = TypeID::value<SpriteComponent>();
		setUVDefaults();
	}

	void setUVDefaults()
	{
		float s = 0.0f;
		float r = 1.0f;
		m_UV.push_back(glm::vec4(s, s, 0, 0));
		m_UV.push_back(glm::vec4(s, r, 0, 0));
		m_UV.push_back(glm::vec4(r, r, 0, 0));
		m_UV.push_back(glm::vec4(r, s, 0, 0));
	}

	void setUV(float x, float y, float width, float height)
	{
		if (m_Texture == nullptr) return;

		float sw = (float)m_Texture->getWidth();
		float sh = (float)m_Texture->getHeight();

		float inverseRow = (sh / height) - y - 1.0f;

		float tx = (x * width) / sw;
		float ty = (inverseRow * height) / sh;
		float tw = (width / sw);
		float th = (height / sh);

		//float sx = 0.2f / sw;
		//float sy = 0.3f / sh;
		float sx = sw;
		float sy = sh;

		m_UV[0] = glm::vec4(tx, ty, sx, sy);
		m_UV[1] = glm::vec4(tx, ty + th, sx, sy);
		m_UV[2] = glm::vec4(tx + tw, ty + th, sx, sy);
		m_UV[3] = glm::vec4(tx + tw, ty, sx, sy);
	}

	~SpriteComponent() {}	

	glm::vec2 m_Size;
	Texture* m_Texture;

	std::vector<glm::vec4> m_UV;
};