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

	template<typename T>
	void add(const T& entity);

	void update(float timeElapsed);
	void render(Renderer& renderer);

private:
	std::vector<std::unique_ptr<Entity>> m_Particles;

};

template<typename T>
inline void ParticleManager::add(const T& entity)
{
	m_Particles.push_back(std::make_unique<T>(entity));
}
