#include "ObjectManager.h"

std::shared_ptr<QTree<Sprite>> ObjectManager::WaterTilesQT = std::shared_ptr<QTree<Sprite>>(new QTree<Sprite>(0, BoundingBox(0, 0, 0, 0)));
std::shared_ptr<QTree<Sprite>> ObjectManager::QuadTree = std::shared_ptr<QTree<Sprite>>(new QTree<Sprite>(0, BoundingBox(0, 0, 0, 0)));
std::shared_ptr<QTree<Sprite>> ObjectManager::ObjectsQT = std::shared_ptr<QTree<Sprite>>(new QTree<Sprite>(0, BoundingBox(0, 0, 0, 0)));

void ObjectManager::init(const BoundingBox& bbox)
{
	ObjectsQT = std::shared_ptr<QTree<Sprite>>(new QTree<Sprite>(0, bbox));
	WaterTilesQT = std::shared_ptr<QTree<Sprite>>(new QTree<Sprite>(0, bbox));
	QuadTree = std::shared_ptr<QTree<Sprite>>(new QTree<Sprite>(0, bbox));
}
