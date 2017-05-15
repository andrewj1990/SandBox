#pragma once

#include "..\Entity.h"
#include "..\..\Graphics\Renderer.h"

class ParticleManager
{
	std::vector<std::unique_ptr<Entity>> particles_;
	std::vector<std::unique_ptr<Entity>> ready_particles_;

public:
	
	static ParticleManager& instance()
	{
		static ParticleManager instance;
		return instance;
	}

	template<class T>
	void add(T&& entity);
	
	template<class T>
	void addReady(T&& entity);

	void update(float timeElapsed);
	void render(Renderer& renderer);


};

template<class T>
inline void ParticleManager::add(T&& entity)
{
	ready_particles_.emplace_back(std::make_unique<T>(entity));
}

template<class T>
inline void ParticleManager::addReady(T&& entity)
{
	ready_particles_.emplace_back(std::make_unique<T>(entity));
}
