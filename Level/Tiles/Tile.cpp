#include "tile.h"

Tile::Tile()
	: Sprite()
{
	m_Texture = TextureManager::get("Textures/Level/level.png");
	setUV(0, 0, 16, 16);
}

Tile::Tile(const glm::vec2& size)
	: Sprite(glm::vec3(0, 0, -(Settings::Instance().Z_PLANE) + 11), size)
{
	m_Texture = TextureManager::get("Textures/Level/level.png");
	setUV(0, 0, 16, 16);
}

void Tile::init(float x, float y, const glm::vec4& colour, bool solid)
{
	m_Position.x = x;
	m_Position.y = y;

	m_CollisionBox = BoundingBox();
	m_CollisionBox.x = x;
	m_CollisionBox.y = y;
	m_CollisionBox.width = Settings::Instance().TILE_SIZE;
	m_CollisionBox.height = Settings::Instance().TILE_SIZE;

	// add small offset on the y position to stop the "z-fighting" when sorting based on y-pos
	//m_Tree->setPosition(x, y + ((int)x % 1000) * 0.001);
}

void Tile::submit(Renderer& renderer) const
{
	renderer.submit(*this);
}

