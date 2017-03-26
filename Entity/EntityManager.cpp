#include "EntityManager.h"


EntityManager::EntityManager()
{
	init();
}

void EntityManager::init()
{
	m_Systems.push_back(std::make_unique<MovementSystem>(MovementSystem()));
}

void EntityManager::update(float timeElapsed)
{
	for (auto i = m_Entities.begin(); i != m_Entities.end(); )
	{
		if ((*i)->shouldDestroy())
		{
			//i = m_Entities.erase(i);
			std::swap(i, m_Entities.end() - 1);
			m_Entities.pop_back();
		}
		else
		{
			for (auto& system : m_Systems)
			{
				if (system->unlock((*i)->getKey()))
				{
					system->update((*i), timeElapsed);
				}
			}

			i++;
		}
	}

}

void EntityManager::render(Renderer& renderer)
{
	//renderer.render(m_Entities);
	renderer.begin();
	for (auto& entity : m_Entities)
	{
		auto& position = entity->get<PositionComponent>();
		auto& sprite = entity->get<SpriteComponent>();

		if (position && sprite)
		{
			renderer.submit(position->m_Position, sprite->m_Size, sprite->m_UV, sprite->m_Texture);
		}
	}

	renderer.end();
	glEnable(GL_DEPTH_TEST);
	renderer.flush();
	glDisable(GL_DEPTH_TEST);
}

void EntityManager::add(TEntity entity)
{
	m_Entities.push_back(std::make_shared<TEntity>(entity));
}
