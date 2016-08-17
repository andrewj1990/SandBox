#include "PlayerUI.h"

PlayerUI::PlayerUI(std::shared_ptr<Player> player)
	: m_Player(player)
{
	m_PlayerHealth = Sprite(glm::vec3(100, 100, 0), glm::vec2(200, 20), glm::vec4(1, 0, 0, 1));
	m_Stamina = Sprite(glm::vec3(100, 100, 0), glm::vec2(200, 20), glm::vec4(0, 1, 0, 1));
}

void PlayerUI::update(float timeElapsed)
{
	const glm::vec3& camPos = Window::Instance().getCamera().getPosition();
	float winHeight = Window::Instance().getHeight();
	m_PlayerHealth.setPosition(camPos.x + 20, camPos.y + winHeight - 40);
	m_Stamina.setPosition(camPos.x + 20, m_PlayerHealth.getPosition().y - 30);
}

void PlayerUI::render(Renderer& renderer)
{
	renderer.render(m_PlayerHealth);
	renderer.render(m_Stamina);
}
