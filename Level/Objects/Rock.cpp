#include "Rock.h"

Rock::Rock(int x, int y)
	: Entity(glm::vec3(x, y, -y), glm::vec2(Utils::random(32, 64), Utils::random(64, 96)), TextureManager::get("Textures/Rock.png"))
{
	float sizeFactorX = getWidth() / 10.0f;
	float sizeFactorY = getHeight() / 10.0f;
	m_CollisionBox = BoundingBox(x + (sizeFactorX * 1), y, (sizeFactorX * 8), sizeFactorY);
	m_Occluder = BoundingBox(x + (sizeFactorX * 1), y, (sizeFactorX * 8), sizeFactorY);
}

void Rock::update(float timeElapsed)
{
}

void Rock::submit(Renderer& renderer) const
{
	renderer.submit(*this);

	if (Settings::Instance().debugShowCollisionBoxes)
	{
		renderer.submit(Sprite(glm::vec3(m_CollisionBox.x, m_CollisionBox.y, getY() + 1), glm::vec2(m_CollisionBox.width, m_CollisionBox.height), TextureManager::get("Textures/collision_box.png")));
	}
}
