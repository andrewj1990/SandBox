#include "tile.h"

Tile::Tile()
	: Renderable()
{
	m_Tree = std::shared_ptr<Entity>(new Entity(glm::vec3(0, 0, 0), glm::vec2(64, 64), TextureManager::get("Textures/Tree.png")));
	m_Tree->getSprite().setUV(0, 0, 10, 10);
	m_TreeTile = true;

	m_Texture = TextureManager::get("Textures/Level/level.png");
	setUV(0, 0, 8, 8);
}

Tile::Tile(const glm::vec2& size)
	: Renderable(glm::vec3(0, 0, 0), size)
{
	m_Tree = std::shared_ptr<Entity>(new Entity(glm::vec3(0, 0, 0), glm::vec2(64, 64), TextureManager::get("Textures/Tree.png")));
	m_Tree->getSprite().setUV(0, 0, 10, 10);
	m_TreeTile = true;

	m_Texture = TextureManager::get("Textures/Level/level.png");
	setUV(0, 0, 8, 8);
}

Tile::Tile(const std::string& tilesheet)
	: Renderable()
{
	m_Tree = std::shared_ptr<Entity>(new Entity(glm::vec3(0, 0, 0), glm::vec2(64, 64), TextureManager::get("Textures/Tree.png")));
	m_Tree->getSprite().setUV(0, 0, 10, 10);
	m_TreeTile = true;

	m_Texture = TextureManager::get(tilesheet);
	setUV(0, 0, m_Texture->getWidth(), m_Texture->getHeight());
}

void Tile::init(float x, float y, const glm::vec4& colour, bool solid, bool treeTile)
{
	m_Position.x = x;
	m_Position.y = y;
	m_CollisionBox->x = x;
	m_CollisionBox->y = y;
	m_CollisionBox->width = Settings::TILE_SIZE;
	m_CollisionBox->height = Settings::TILE_SIZE;

	//m_Colour = colour;

	m_Solid = colour.z;
	setUV(colour.x, colour.y, 8, 8);

	m_TreeTile = treeTile;

	// add small offset on the y position to stop the "z-fighting" when sorting based on y-pos
	m_Tree->getSprite().setPosition(x, y + ((int)x % 1000) * 0.001);
}

void Tile::submit(Renderer& renderer) const
{
	renderer.submit(*this);
}

