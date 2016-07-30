#include "Boss.h"

Boss::Boss(float x, float y)
	: Entity(glm::vec3(x, y, 0), glm::vec2(32, 32), TextureManager::get("Textures/Player/PlayerSpritesheet10.png"))
{
	m_Actions.push_back(Utils::getRandomAction());
	m_ActionIndex = -1;
}

void Boss::damage(int amount)
{
}

void Boss::update(const Terrain& terrain, float timeElapsed)
{
	// use random action
	if (m_ActionIndex == -1 && m_Actions.size() > 0)
	{
		m_ActionIndex = Utils::random(0, m_Actions.size());
	}
	else
	{
		m_Actions[m_ActionIndex]->play(*this);
	}

}

void Boss::update(float timeElapsed)
{
}

void Boss::render(Renderer& renderer)
{
}
