#include "ParticleManager.h"


void ParticleManager::update(float timeElapsed)
{
	while (!ready_particles_.empty())
	{
		particles_.emplace_back(std::move(ready_particles_.back()));
		ready_particles_.pop_back();
	}

	for (auto it = particles_.begin(); it != particles_.end(); )
	{
		if ((*it)->shouldDestroy())
		{
			it = particles_.erase(it);
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

	for (auto& particle : particles_)
	{
		particle->submit(renderer);
	}

	renderer.end();
	renderer.flush();
}
