#pragma once

#include <vector>
#include <memory>

#include "TEntity.h"

class EntityManager
{
public:
	EntityManager();

	void update();
	void add(std::shared_ptr<TEntity> entity);

	std::vector<std::shared_ptr<TEntity>>& getEntities() { return m_Entities; }

private:
	std::vector<std::shared_ptr<TEntity>> m_Entities;

};