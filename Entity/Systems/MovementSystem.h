#pragma once

#include <iostream>

#include "System.h"
//#include "..\Components\PositionComponent.h"

class MovementSystem : public System
{
public:
	MovementSystem();
	~MovementSystem();

	void update(std::shared_ptr<TEntity>& entity, float timeElapsed) override;
private:

};