#include "tile.h"

Tile::Tile()
	: Renderable()
{
	m_Tree = std::shared_ptr<Entity>(new Entity(glm::vec3(0, 0, 0), glm::vec2(64, 64), TextureManager::get("Textures/Tree.png")));
	m_TreeTile = true;
}

void Tile::init(float x, float y, const glm::vec4& colour, bool solid, bool treeTile)
{
	m_Position.x = x;
	m_Position.y = y;

	m_Colour = colour;

	m_Solid = solid;
	m_TreeTile = treeTile;

	// add small offset on the y position to stop the "z-fighting" when sorting based on y-pos
	m_Tree->getSprite().setPosition(x, y + ((int)x % 1000) * 0.001);
}

void Tile::submit(Renderer& renderer) const
{
	renderer.submit(*this);
}

