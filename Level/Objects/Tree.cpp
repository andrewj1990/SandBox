#include "Tree.h"

Tree::Tree(int x, int y)
	: Sprite(glm::vec3(x, y, -y), glm::vec2(Utils::random(32, 64), Utils::random(64, 96)), TextureManager::get("Textures/Tree.png"))
{
	float sizeFactorX = getWidth() / 10.0f;
	float sizeFactorY = getHeight() / 10.0f;
	m_CollisionBox = std::shared_ptr<BoundingBox>(new BoundingBox(x + (sizeFactorX * 3), y, (sizeFactorX * 3), sizeFactorY));
}

void Tree::update(float timeElapsed)
{
}

void Tree::submit(Renderer& renderer) const
{
	renderer.submit(*this);

	if (Settings::Instance().debugShowCollisionBoxes)
	{
		renderer.submit(Sprite(glm::vec3(m_CollisionBox->x, m_CollisionBox->y, getY() + 1), glm::vec2(m_CollisionBox->width, m_CollisionBox->height), TextureManager::get("Textures/collision_box.png")));
	}
}
