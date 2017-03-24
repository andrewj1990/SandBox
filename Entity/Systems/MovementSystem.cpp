#include "MovementSystem.h"

MovementSystem::MovementSystem(std::shared_ptr<EntityManager>& entityManager)
	: System(entityManager)
{
	m_Lock = 1 << TypeID::value<PositionComponent>() | 1 << TypeID::value<VelocityComponent>();
}

MovementSystem::~MovementSystem()
{
}

void MovementSystem::update(float timeElapsed)
{
	for (auto& entity : m_EntityManager->getEntities())
	{
		if (unlock(entity->getKey()))
		{
			//std::cout << "unlocked\n";
		}
		auto positionComponent = entity->get<PositionComponent>();

		if (positionComponent)
		{
			//std::cout << positionComponent->m_X << ", " << positionComponent->m_Y << "\n";
		}

		auto velocityComponent = entity->get<VelocityComponent>();
		if (velocityComponent)
		{
			positionComponent->m_X += velocityComponent->m_X;
			//if (positionComponent->m_X > 300) {
			//	entity->setDestroy(true);
			//}
		}
	}
}
