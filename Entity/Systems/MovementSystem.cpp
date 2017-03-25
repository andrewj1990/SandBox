#include "MovementSystem.h"

MovementSystem::MovementSystem()
	: System()
{
	m_Lock = 1 << TypeID::value<PositionComponent>() | 1 << TypeID::value<VelocityComponent>();
}

MovementSystem::~MovementSystem()
{
}

void MovementSystem::update(std::shared_ptr<TEntity>& entity, float timeElapsed)
{
	auto positionComponent = entity->get<PositionComponent>();
	//std::cout << positionComponent->m_X << ", " << positionComponent->m_Y << "\n";

	auto velocityComponent = entity->get<VelocityComponent>();
	positionComponent->m_X += velocityComponent->m_X;
}
