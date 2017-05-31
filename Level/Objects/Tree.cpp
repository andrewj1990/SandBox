#include "Tree.h"

Tree::Tree(int x, int y)
	: Entity(glm::vec3(x, y, -y), glm::vec2(Utils::random(64, 128)), TextureManager::get("Textures/tree_top4.png"))
{
	float sizeFactorX = getWidth() / 10.0f;
	float sizeFactorY = getHeight() / 10.0f;
	m_CollisionBox = BoundingBox(x + (sizeFactorX * 3), y + sizeFactorY * 3, (sizeFactorX * 3), sizeFactorY * 3);
	m_Occluder = BoundingBox(x + (sizeFactorX * 3), y + sizeFactorY * 3, (sizeFactorX * 3), sizeFactorY * 3);

	m_Angle = glm::radians(Utils::random(0.0f, 360.0f));
	transform_ = glm::translate(transform_, glm::vec3(x + getWidth() / 2, y + getHeight() / 2, 0));
	transform_ = glm::rotate(transform_, m_Angle, glm::vec3(0, 0, 1));
	transform_ = glm::translate(transform_, glm::vec3(-x - getWidth() / 2, -y - getHeight() / 2, 0));
}

void Tree::update(float timeElapsed)
{
}

void Tree::submit(Renderer& renderer) 
{
	renderer.push(transform_);
	renderer.submit(*this);
	renderer.pop();

	if (Settings::Instance().debugShowCollisionBoxes)
	{
		renderer.submit(Sprite(glm::vec3(m_CollisionBox.x, m_CollisionBox.y, getY() + 1), glm::vec2(m_CollisionBox.width, m_CollisionBox.height), TextureManager::get("Textures/collision_box.png")));
	}
}
