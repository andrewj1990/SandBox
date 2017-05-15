#pragma once

#include <memory>
#include "../Utils/QTree.h"
#include "../Graphics/Sprite.h"
#include "../Entity/Entity.h"

class ObjectManager
{
public:
	
	static void init(const BoundingBox& bbox);

	static std::unique_ptr<QTree<Sprite>> WaterTilesQT;
	static std::unique_ptr<QTree<Sprite>> QuadTree;
	static std::unique_ptr<QTree<Entity>> ObjectsQT;
	static std::unique_ptr<QTree<Entity>> MobQT;

};