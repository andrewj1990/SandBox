#include "MoveAction.h"
#include "..\..\Entity.h"

MoveAction::MoveAction()
	: Action()
{
}

void MoveAction::play(Entity& entity)
{
	Window& window = Window::Instance();
	float playerX = window.getCamera().getPosition().x + window.getWidth() / 2;
	float playerY = window.getCamera().getPosition().y + window.getHeight() / 2;

	float mobX = entity.getCenterX();
	float mobY = entity.getCenterY();

	float dx = playerX - mobX;
	float dy = playerY - mobY;

	float angle = std::atan2f(dy, dx);

	// if collides with player then set action as complete

	entity.getSprite().addDirection(std::cosf(angle), std::sinf(angle));
}
