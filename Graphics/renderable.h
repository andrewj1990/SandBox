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
		: m_Position(glm::vec3(0, 0, 0)), m_Size(32, 32), m_Colour(glm::vec4(1, 1, 1, 1)), m_Texture(nullptr)
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

	~Renderable()
	{
		//m_Texture = nullptr;
		//delete m_Texture;
	}

	virtual void init(float x, float y, const glm::vec4& colour, bool solid, bool treeTile) {}

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

	void setColor(const glm::vec4& colour)
	{
		m_Colour = colour;
	}

	void addDirection(float dx, float dy)
	{
		m_Position.x += dx;
		m_Position.y += dy;
	}
	
	void setUV(float x, float y, float width, float height)
	{
		if (m_Texture == nullptr) return;

		float sw = m_Texture->getWidth();
		float sh = m_Texture->getHeight();

		float inverseRow = (sh / height) - y - 1;

		float tx = (x * width) / sw;
		float ty = (inverseRow * height) / sh;
		float tw = (width / sw);
		float th = (height / sh);

		float sx = 0.2f / sw;
		float sy = 0.3f / sh;
		
		m_UV[0] = glm::vec4(tx, ty, sx, sy);
		m_UV[1] = glm::vec4(tx, ty + th, sx, sy);
		m_UV[2] = glm::vec4(tx + tw, ty + th, sx, sy);
		m_UV[3] = glm::vec4(tx + tw, ty, sx, sy);
	}

	float getTextureWidth() const
	{
		return m_Texture == nullptr ? 0.0f : m_Texture->getWidth();
	}

	float getTextureHeight() const
	{
		return m_Texture == nullptr ? 0.0f : m_Texture->getHeight();
	}

	void setTexture(Texture* texture) { m_Texture = texture; m_UV = m_Texture->getUVs(); }

	virtual std::shared_ptr<Entity> getTree() { return nullptr; }
	void setPosition(const glm::vec3& position) { m_Position = position; }
	inline const glm::vec3& getPosition() const { return m_Position; }
	inline const glm::vec2& getSize() const { return m_Size; }
	inline const glm::vec4& getColour() const { return m_Colour; }
	inline void setSolid(bool solid) { m_Solid = solid; }
	inline bool isSolid() const { return m_Solid; }
	virtual bool isTreeTile() const { return false; }
	inline const std::vector<glm::vec4>& getUV() const { return m_UV; }
	inline unsigned int getTID() const { return m_Texture == nullptr ? 0 : m_Texture->getTID(); }

protected:	

private:
	void setUVDefaults()
	{
		float s = 0.0f;
		float r = 1.0f;
		m_UV.push_back(glm::vec4(s, s, 0, 0));
		m_UV.push_back(glm::vec4(s, r, 0, 0));
		m_UV.push_back(glm::vec4(r, r, 0, 0));
		m_UV.push_back(glm::vec4(r, s, 0, 0));
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

		m_UV.push_back(glm::vec4(tx, ty, 0, 0));
		m_UV.push_back(glm::vec4(tx, ty + th, 0, 0));
		m_UV.push_back(glm::vec4(tx + tw, ty + th, 0, 0));
		m_UV.push_back(glm::vec4(tx + tw, ty, 0, 0));
	}

protected:
	glm::vec3 m_Position;
	glm::vec2 m_Size;
	glm::vec4 m_Colour;
	std::vector<glm::vec4> m_UV;
	bool m_Solid;

	Texture* m_Texture;
	//std::shared_ptr<Texture> m_Texture;

};