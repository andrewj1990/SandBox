#pragma once

#include <vector>
#include <memory>

#include "..\Graphics\Renderer.h"
#include "Systems\MovementSystem.h"

class EntityManager
{
public:
	EntityManager();

	void update(float timeElapsed);
	void render(Renderer& renderer);

	void add(std::shared_ptr<TEntity> entity);

	std::vector<std::shared_ptr<TEntity>>& getEntities() { return m_Entities; }

private:
	void init();

private:
	std::vector<std::shared_ptr<TEntity>> m_Entities;
	std::vector<std::unique_ptr<System>> m_Systems;

};