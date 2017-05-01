#include "ParticleManager.h"


void ParticleManager::update(float timeElapsed)
{
	while (!m_ReadyParticles.empty())
	{
		m_Particles.emplace_back(std::move(m_ReadyParticles.back()));
		m_ReadyParticles.pop_back();
	}

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
