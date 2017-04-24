#include "ParticleManager.h"


//void ParticleManager::add(const Entity& entity)
//{
//	m_Particles.push_back(std::make_unique<Entity>(entity));
//}

void ParticleManager::update(float timeElapsed)
{
	for (auto it = m_Particles.begin(); it != m_Particles.end(); )
	{
		if ((*it)->shouldDestroy())
		{
			it = m_Particles.erase(it);
		}
		else
		{
			(*it)->update(timeElapsed);
			++it;
		}

	}
}

void ParticleManager::render(Renderer& renderer)
{
	renderer.begin();

	for (auto& particle : m_Particles)
	{
		particle->submit(renderer);
	}

	renderer.end();
	renderer.flush();
}
