#pragma once

#include "..\..\Entity\TEntity.h"
#include "..\..\Entity\Components\PositionComponent.h"

class ObjectFactory
{
public:
	static TEntity createTree(int x, int y);

};