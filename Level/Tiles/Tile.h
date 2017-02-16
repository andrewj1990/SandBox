#pragma once

#include "..\..\Graphics\renderable.h"
#include "..\..\Entity\Entity.h"

enum TileType
{
	GRASS,
	SHALLOW_WATER,
	DEEP_WATER,
	SAND,
	VOID
};

class Tile : public Sprite
{
public:
	Tile();
	Tile(const glm::vec2& size);

	void init(float x, float y, const glm::vec4& colour, bool solid);
	void submit(Renderer& renderer) const override;

	void setType(TileType type) { m_TileType = type; }
	TileType getType() const { return m_TileType; }

private:
	TileType m_TileType;

};