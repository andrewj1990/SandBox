#include "ObjectManager.h"

std::unique_ptr<QTree<Sprite>> ObjectManager::WaterTilesQT = std::make_unique<QTree<Sprite>>(0, BoundingBox(0, 0, 0, 0));
std::unique_ptr<QTree<Sprite>> ObjectManager::QuadTree = std::make_unique<QTree<Sprite>>(0, BoundingBox(0, 0, 0, 0));
std::unique_ptr<QTree<Entity>> ObjectManager::ObjectsQT = std::make_unique<QTree<Entity>>(0, BoundingBox(0, 0, 0, 0));
std::unique_ptr<QTree<Entity>> ObjectManager::MobQT = std::make_unique<QTree<Entity>>(0, BoundingBox(0, 0, 0, 0));

void ObjectManager::init(const BoundingBox& bbox)
{
	ObjectsQT	 = std::make_unique<QTree<Entity>>(0, bbox);
	MobQT		 = std::make_unique<QTree<Entity>>(0, bbox);
	WaterTilesQT = std::make_unique<QTree<Sprite>>(0, bbox);
	QuadTree	 = std::make_unique<QTree<Sprite>>(0, bbox);
}
