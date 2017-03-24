#pragma once

#include <iostream>

#include "System.h"
#include "..\Components\PositionComponent.h"

class MovementSystem : public System
{
public:
	MovementSystem(std::shared_ptr<EntityManager>& entityManager);
	~MovementSystem();

	void update(float timeElapsed) override;

private:

};