#include "EntityManager.h"


EntityManager::EntityManager()
{
}

void EntityManager::update()
{
	for (auto i = m_Entities.begin(); i != m_Entities.end(); )
	{
		if ((*i)->shouldDestroy())
		{
			i = m_Entities.erase(i);
		}
		else
		{
			i++;
		}
	}
}

void EntityManager::add(std::shared_ptr<TEntity> entity)
{
	m_Entities.push_back(entity);
}
