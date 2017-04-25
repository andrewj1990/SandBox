#include "Bush.h"

Bush::Bush(int x, int y)
	: Entity(glm::vec3(x, y, -y), glm::vec2(Utils::random(32, 64), Utils::random(64, 96)), TextureManager::get("Textures/Bush.png"))
{
	m_Solid = false;
	float sizeFactorX = getWidth() / 10.0f;
	float sizeFactorY = getHeight() / 10.0f;
	m_CollisionBox = std::shared_ptr<BoundingBox>(new BoundingBox(x + (sizeFactorX * 1), y, (sizeFactorX * 8), 1));
	m_Occluder = std::shared_ptr<BoundingBox>(new BoundingBox(x + (sizeFactorX * 1), y, (sizeFactorX * 8), 1));
}

void Bush::update(float timeElapsed)
{
}

void Bush::submit(Renderer& renderer) const
{
	renderer.submit(*this);

	if (Settings::Instance().debugShowCollisionBoxes)
	{
		renderer.submit(Sprite(glm::vec3(m_CollisionBox->x, m_CollisionBox->y, getY() + 1), glm::vec2(m_CollisionBox->width, m_CollisionBox->height), TextureManager::get("Textures/collision_box.png")));
	}
}
