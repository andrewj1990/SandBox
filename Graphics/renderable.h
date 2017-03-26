#pragma once

#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <vector>
#include "texture.h"
#include "Renderer.h"
#include "..\Utils\Utils.h"
#include "..\Utils\BoundingBox.h"
#include "texture_manager.h"

class Renderable
{
public:
	Renderable()
		: m_Position(glm::vec3(0, 0, 0)), m_Size(32, 32), m_Colour(glm::vec4(1, 1, 1, 1)), m_Texture(nullptr)
	{
		setUVDefaults();
	}

	Renderable(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1, 1, 1, 1))
		: m_Position(position), m_Size(size), m_Colour(color), m_Texture(nullptr)
	{
		setUVDefaults();
	}

	Renderable(const glm::vec3& position, const glm::vec2& size, Texture* texture)
		: m_Position(position), m_Size(size), m_Colour(glm::vec4(1, 1, 1, 1)), m_Texture(texture)
	{
		setUVDefaults();
	}

	// quad in ccw position where x0,y0 is bottom left
	Renderable(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, const glm::vec4& color = glm::vec4(1, 1, 1, 1))
		: m_Colour(color), m_Texture(nullptr)
	{
		setPositions(x0, y0, x1, y1, x2, y2, x3, y3);
		setUVDefaults();
	}

	virtual ~Renderable()
	{
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

	void setColor(const glm::vec4& colour)
	{
		m_Colour = colour;
	}

	void setSize(const glm::vec2& size)
	{
		m_Size = size;
	}

	void scaleUp(float scale)
	{
		m_Size *= scale;
	}

	void setDepth(int depth)
	{
		m_Position.z = depth;
	}

	void addDirection(float dx, float dy)
	{
		m_Position.x += dx;
		m_Position.y += dy;
		m_Position.z = -m_Position.y;
	}

	// store light position in uv
	void setLightPosition(float x, float y, float radius)
	{
		m_UV[0] = glm::vec4(x, y, radius, 0);
		m_UV[1] = glm::vec4(x, y, radius, 0);
		m_UV[2] = glm::vec4(x, y, radius, 0);
		m_UV[3] = glm::vec4(x, y, radius, 0);
	}

	void setUV(float x, float y, float width, float height)
	{
		if (m_Texture == nullptr) return;

		float sw = (float)m_Texture->getWidth();
		float sh = (float)m_Texture->getHeight();

		float inverseRow = (sh / height) - y - 1.0f;

		float offsetX = 0.1f / width;
		float offsetY = 0.1f / height;
		float tx = (x * width) / sw;
		float ty = (inverseRow * height) / sh;
		float tw = (width / sw);
		float th = (height / sh);

		//float sx = 0.2f / sw;
		//float sy = 0.3f / sh;
		float sx = sw;
		float sy = sh;

		m_UV[0] = glm::vec4(tx + offsetX	 , ty + offsetY	  , sx, sy);
		m_UV[1] = glm::vec4(tx + offsetX	 , ty + th - offsetY, sx, sy);
		m_UV[2] = glm::vec4(tx + tw - offsetX, ty + th - offsetY, sx, sy);
		m_UV[3] = glm::vec4(tx + tw - offsetX, ty + offsetY, sx, sy);
	}

	float getTextureWidth() const
	{
		return m_Texture == nullptr ? 0.0f : m_Texture->getWidth();
	}

	float getTextureHeight() const
	{
		return m_Texture == nullptr ? 0.0f : m_Texture->getHeight();
	}

	void setPositions(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3)
	{
		m_Positions.clear();
		m_Positions.push_back(glm::vec3(x0, y0, 0));
		m_Positions.push_back(glm::vec3(x1, y1, 0));
		m_Positions.push_back(glm::vec3(x2, y2, 0));
		m_Positions.push_back(glm::vec3(x3, y3, 0));
	}

	void setTexture(Texture* texture) { m_Texture = texture; m_UV = m_Texture->getUVs(); }
	Texture* getTexture() { return m_Texture; }

	virtual std::shared_ptr<Entity> getTree() { return nullptr; }
	void setPosition(const glm::vec3& position) { m_Position = position; }
	inline const glm::vec3& getPosition() const { return m_Position; }
	inline const std::vector<glm::vec3>& getPositions() const { return m_Positions; }
	inline const glm::vec2& getSize() const { return m_Size; }
	inline const glm::vec4& getColour() const { return m_Colour; }
	virtual bool isTreeTile() const { return false; }
	inline const std::vector<glm::vec4>& getUV() const { return m_UV; }
	inline unsigned int getTID() const { return m_Texture == nullptr ? 0 : m_Texture->getTID(); }

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
	std::vector<glm::vec3> m_Positions;
	glm::vec2 m_Size;
	glm::vec4 m_Colour;
	std::vector<glm::vec4> m_UV;

	Texture* m_Texture;
};