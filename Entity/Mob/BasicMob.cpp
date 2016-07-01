#include "BasicMob.h"

BasicMob::BasicMob(float x, float y)
	: Entity(x, y)
{
	m_Sprite = Sprite(glm::vec3(x, y, 0), glm::vec2(32, 32), TextureManager::get("Textures/Player/PlayerSpritesheet10.png"));
}

BasicMob::~BasicMob()
{
}

void BasicMob::update(float timeElapsed)
{
	m_CumulativeTime += timeElapsed;
	if (m_CumulativeTime > 1.0f)
	{
		m_CumulativeTime = 0;
		m_Dx = 1.0f - (rand() % 1000 / 500.0f);
		m_Dy = 1.0f - (rand() % 1000 / 500.0f);
	}

	m_Sprite.addDirection(m_Dx, m_Dy);

}

void BasicMob::render(Renderer& renderer)
{
	renderer.render(m_Sprite);
}
