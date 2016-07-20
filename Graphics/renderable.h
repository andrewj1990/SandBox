#pragma once

#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <vector>
#include "texture.h"
#include "Renderer.h"
#include "..\Utils\Utils.h"

class Terrain;

class Renderable
{
protected:
	Renderable()
	{
		setUVDefaults();
	}

public:
	Renderable(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1, 1, 1, 1))
		: m_Position(position), m_Size(size), m_Colour(color), m_Texture(nullptr)
	{	
		m_Solid = false;
		setUVDefaults();
	}

	Renderable(const glm::vec3& position, const glm::vec2& size, Texture* texture)
		: m_Position(position), m_Size(size), m_Colour(glm::vec4(1, 1, 1, 1)), m_Texture(texture)
	{
		m_Solid = false;
		setUVDefaults();
	}

	Renderable(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, const glm::vec4& color = glm::vec4(1, 1, 1, 1))
		: m_Colour(color)
	{
		m_Solid = false;
		setUVDefaults();
	}

	virtual void submit(Renderer& renderer) const
	{
		renderer.submit(*this);
	}

	void setColor(float r, float g, float b)
	{
		m_Colour.r = r;
		m_Colour.g = g;
		m_Colour.b = b;
	}

	void setColor(const glm::vec3& color)
	{
		m_Colour = glm::vec4(color, 1);
	}

	void addDirection(float dx, float dy)
	{
		m_Position.x += dx;
		m_Position.y += dy;
	}


	void setTexture(Texture* texture) { m_Texture = texture; m_UV = m_Texture->getUVs(); }

	void setPosition(const glm::vec3& position) { m_Position = position; }
	inline const glm::vec3& getPosition() const { return m_Position; }
	inline const glm::vec2& getSize() const { return m_Size; }
	inline const glm::vec4& getColour() const { return m_Colour; }
	inline void setSolid(bool solid) { m_Solid = solid; }
	inline bool isSolid() const { return m_Solid; }
	inline const std::vector<glm::vec2>& getUV() const { return m_UV; }
	inline const unsigned int getTID() const { return m_Texture == nullptr ? 0 : m_Texture->getTID(); }

	virtual float getAngle() { return 0; }
	virtual void damage(int amount) {}
	virtual void setDestroy(bool destroy) {}
	virtual bool shouldDestroy() const { return false; }

	virtual void update(const Terrain& terrain, float timeElapsed) {}
	virtual void update(float timeElapsed) {}
	virtual void render(Renderer& renderer) {}

protected:	

private:
	void setUVDefaults()
	{
		float s = 0.0f;
		float r = 1.0f;
		m_UV.push_back(glm::vec2(s, s));
		m_UV.push_back(glm::vec2(s, r));
		m_UV.push_back(glm::vec2(r, r));
		m_UV.push_back(glm::vec2(r, s));
	}

	void setUV()
	{
		float totalSize = 64;
		float size = 22;

		float ix = 1;
		float iy = 1;

		float tx = (ix * size) / totalSize;
		float ty = (iy * size) / totalSize;
		float tw = (size / totalSize);
		float th = (size / totalSize);

		m_UV.push_back(glm::vec2(tx, ty));
		m_UV.push_back(glm::vec2(tx, ty + th));
		m_UV.push_back(glm::vec2(tx + tw, ty + th));
		m_UV.push_back(glm::vec2(tx + tw, ty));
	}

protected:
	glm::vec3 m_Position;
	glm::vec2 m_Size;
	glm::vec4 m_Colour;
	std::vector<glm::vec2> m_UV;
	bool m_Solid;

	Texture* m_Texture;

};