#pragma once

#include <memory>
#include "../Utils/QTree.h"
#include "../Graphics/Sprite.h"

class ObjectManager
{
public:
	static std::shared_ptr<QTree<Sprite>> WaterTilesQT;
	static std::shared_ptr<QTree<Sprite>> QuadTree;
	static std::shared_ptr<QTree<Sprite>> ObjectsQT;

};