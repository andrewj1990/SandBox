#include "Gun.h"

Gun::Gun(float x, float y)
	: m_Sprite(glm::vec3(x, y, 0), glm::vec2(32, 32), TextureManager::get("Textures/Player/Gun.png"))
{
	m_X = x;
	m_Y = y;
}

void Gun::move(float x, float y)
{
	m_Sprite.addDirection(x, y);
}

void Gun::update(float timeElapsed)
{
	float mx = Window::Instance().mouseX();
	float my = Window::Instance().mouseY();

	float dx = mx - m_X;
	float dy = my - m_Y;
	float angle = -std::atan2f(dy, dx) - glm::radians(45.0f);

	m_Sprite.setAngle(angle);
}

void Gun::render(Renderer& renderer)
{
	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(m_Sprite.getPosition().x + 5, m_Sprite.getPosition().y + 5, 0));
	transform = glm::rotate(transform, m_Sprite.getAngle(), glm::vec3(0, 0, 1));
	transform = glm::translate(transform, glm::vec3(-m_Sprite.getPosition().x - 5, -m_Sprite.getPosition().y - 5, 0));
	
	renderer.push(transform);
	renderer.render(m_Sprite);
	renderer.pop();
}
