#include "DamageCounter.h"

DamageCounter::DamageCounter(const std::string& text, float x, float y)
	: Entity(x, y), m_Label(text, x, y, FontManager::get("Arial"), glm::vec4(1, 1, 1, 1))
{
	m_Dx = 1.0f - (rand() % 1000 / 500.0f);
}

void DamageCounter::update(float timeElapsed)
{
	m_Counter += timeElapsed * 5;
	if (m_Counter > 5.4f)
	{
		m_Destroy = true;
		return;
	}

	//m_Label.position.y += std::sinf(m_Counter) * 3;
	//m_Label.position.x += m_Dx;
	m_Label.addDirection(m_Dx, std::sinf(m_Counter) * 3);
	
}

void DamageCounter::render(Renderer& renderer)
{
	renderer.render(m_Label);
}

void DamageCounter::submit(Renderer& renderer)
{
	m_Label.submit(renderer);
}
