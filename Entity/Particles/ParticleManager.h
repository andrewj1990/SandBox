#pragma once

#include "..\Entity.h"
#include "..\..\Graphics\Renderer.h"

class ParticleManager
{
	std::vector<std::unique_ptr<Entity>> particles_;
	std::vector<std::unique_ptr<Entity>> ready_particles_;
	std::vector<std::unique_ptr<Entity>> render_once_particles_;

public:
	
	static ParticleManager& instance()
	{
		static ParticleManager instance;
		return instance;
	}

	template<class T>
	void add(const T& entity);
	
	template<class T>
	void addReady(const T& entity);

	void update(float timeElapsed);
	void render(Renderer& renderer);
	void renderOnce(Renderer& renderer);

};

template<class T>
inline void ParticleManager::add(const T& entity)
{
	ready_particles_.emplace_back(std::make_unique<T>(entity));
}

template<class T>
inline void ParticleManager::addReady(const T& entity)
{
	ready_particles_.emplace_back(std::make_unique<T>(entity));
}
