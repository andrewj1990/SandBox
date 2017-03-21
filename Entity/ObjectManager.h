#pragma once

#include <memory>
#include "../Utils/QTree.h"
#include "../Graphics/Sprite.h"

class ObjectManager
{
public:
	
	static void init(const BoundingBox& bbox);

	static std::shared_ptr<QTree<Sprite>> WaterTilesQT;
	static std::shared_ptr<QTree<Sprite>> QuadTree;
	static std::shared_ptr<QTree<Sprite>> ObjectsQT;

};