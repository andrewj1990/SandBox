#include "MoveAction.h"
#include "..\..\Entity.h"

MoveAction::MoveAction()
	: Action()
{
}

void MoveAction::init()
{
	m_CumulativeTime = 0.0f;
	m_MoveDuration = 1.0f;
	m_Complete = false;
}

void MoveAction::play(Entity& mob, Entity& player, float timeElapsed)
{
	m_CumulativeTime += timeElapsed;

	if (m_CumulativeTime > m_MoveDuration)
	{
		m_Complete = true;
		return;
	}

	Window& window = Window::Instance();
	float playerX = window.getCamera().getPosition().x + window.getWidth() / 2;
	float playerY = window.getCamera().getPosition().y + window.getHeight() / 2;

	float mobX = mob.getCenterX();
	float mobY = mob.getCenterY();

	float dx = playerX - mobX;
	float dy = playerY - mobY;

	float angle = std::atan2f(dy, dx);

	// if collides with player then set action as complete
	if (mob.collide(player))
	{
		m_Complete = true;
	}
	else
	{
		mob.getSprite().addDirection(std::cosf(angle), std::sinf(angle));
	}

}
