#include "ObjectFactory.h"

TEntity ObjectFactory::createTree(int x, int y)
{
	TEntity tree = TEntity();
	tree.attach<PositionComponent>(std::make_shared<PositionComponent>(glm::vec3(x, y, -y)));
	tree.attach<SpriteComponent>(std::make_shared<SpriteComponent>(glm::vec2(Utils::random(32, 64), Utils::random(64, 96)), TextureManager::get("Textures/Tree.png")));

	return tree;
}
