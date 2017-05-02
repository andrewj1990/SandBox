#pragma once

#include "..\Entity.h"
#include "..\..\Graphics\Renderer.h"

class ParticleManager
{
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

private:
	std::vector<std::unique_ptr<Entity>> m_Particles;
	std::vector<std::unique_ptr<Entity>> m_ReadyParticles;

};

template<class T>
inline void ParticleManager::add(T&& entity)
{
	m_ReadyParticles.emplace_back(std::make_unique<T>(entity));
}

template<class T>
inline void ParticleManager::addReady(T&& entity)
{
	m_ReadyParticles.emplace_back(std::make_unique<T>(entity));
}
