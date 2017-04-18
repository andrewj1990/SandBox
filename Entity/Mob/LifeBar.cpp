#include "LifeBar.h"

LifeBar::LifeBar(float width)
	: m_Background(glm::vec3(0, 0, 0), glm::vec2(width, 5), TextureManager::get("Textures/Mobs/life_bar_bg.png")),
	m_Bar(glm::vec3(0, 0, 0), glm::vec2(width - 2, 3), glm::vec4(0, 1, 0, 0.9f)),
	m_DamageBar(glm::vec3(0, 0, 0), glm::vec2(width - 2, 3), glm::vec4(1.0f, 0, 0, 1.0f))
{
	m_BarWidth = width - 2;
}

void LifeBar::setPosition(float x, float y)
{
	m_Background.setPosition(x, y);
	m_Bar.setPosition(x + 1, y + 1);
	m_DamageBar.setPosition(x + 1, y + 1);
}

void LifeBar::update(float percent, float timeElapsed)
{
	if (m_DamageBarWidth > (m_BarWidth * percent))
	{
		m_DamageBarWidth -= timeElapsed * 50;
	}
	else
	{
		m_DamageBarWidth = m_BarWidth * percent;
	}

	if (m_DamageBarWidth > m_BarWidth) m_DamageBarWidth = m_BarWidth;

	m_Bar.setSize(glm::vec2(m_BarWidth * percent, m_Bar.getSize().y));
	m_DamageBar.setSize(glm::vec2(m_DamageBarWidth, m_Bar.getSize().y));
}

void LifeBar::render(Renderer& renderer)
{
	renderer.m_AlphaTest = false;

	renderer.begin();
	renderer.submit(m_Background);
	renderer.submit(m_DamageBar);
	renderer.submit(m_Bar);
	renderer.end();
	renderer.flush();

	renderer.m_AlphaTest = true;
}
