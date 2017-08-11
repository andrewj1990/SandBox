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
			render_once_particles_.emplace_back(std::move(*it));
			//glm::vec4 colour = render_once_particles_.back()->getColour();
			//colour.w = 1.0f;
			//render_once_particles_.back()->setColor(colour);
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

void ParticleManager::renderOnce(Renderer& renderer)
{
	renderer.begin();

	while (!render_once_particles_.empty()) {
		render_once_particles_.back()->submit(renderer);
		render_once_particles_.pop_back();
	}

	for (auto it = render_once_particles_.begin(); it != render_once_particles_.end(); )
	{
		(*it)->submit(renderer);
		it = render_once_particles_.erase(it);
	}

	renderer.end();
	renderer.flush();
}
