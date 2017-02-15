#include "WaterRipple.h"

WaterRipple::WaterRipple(float x, float y, float vel)
{
	for (int i = 0; i < 360; i += 5)
	{
		m_WaterParticles.push_back(std::unique_ptr<WaterParticle>(new WaterParticle(x, y, i, vel)));
	}

}

bool WaterRipple::complete()
{
	return m_WaterParticles.size() == 0;
}

void WaterRipple::update(Region& region, float timeElapsed)
{
	for (auto it = m_WaterParticles.begin(); it != m_WaterParticles.end(); )
	{
		if ((*it)->shouldDestroy())
		{
			it = m_WaterParticles.erase(it);
		}
		else
		{
			(*it)->update(region, timeElapsed);
			it++;
		}
	}
}

void WaterRipple::render(Renderer& renderer)
{
	renderer.begin();
	for (auto& waterParticle : m_WaterParticles)
	{
		waterParticle->submit(renderer);
	}
	renderer.end();
	renderer.flush();
}
