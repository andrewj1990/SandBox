#include "PlayerUI.h"

PlayerUI::PlayerUI(const std::unique_ptr<Player>& player)
	: m_Player(player)
{
	m_PlayerHealth = Sprite(glm::vec3(20, Settings::Instance().PROJECTION_HEIGHT - 40, 0), glm::vec2(200, 20), glm::vec4(1, 0, 0, 1));
	m_Stamina = Sprite(glm::vec3(m_PlayerHealth.getPosition().x, m_PlayerHealth.getPosition().y - 30, 0), glm::vec2(200, 20), glm::vec4(0, 1, 0, 1));
	m_Crosshair = Sprite(glm::vec3(0, 0, 0), glm::vec2(32, 32), TextureManager::get("Textures/Player/crosshair.png"));
}

void PlayerUI::update()
{
	//const glm::vec3& camPos = Window::Instance().getCamera().getPosition();
	//float winHeight = Settings::Instance().PROJECTION_HEIGHT;

	//m_PlayerHealth.setPosition(camPos.x + 20, camPos.y + winHeight - 40);
	//m_Stamina.setPosition(camPos.x + 20, m_PlayerHealth.getPosition().y - 30);
	m_PlayerHealth.setSize(glm::vec2(m_Player->getHPPercent() * 200, 20));

	Window& win = Window::Instance();
	////m_Crosshair.setPosition(win.getCamera().Position.x + win.mouseX(), win.getCamera().Position.y + (win.getHeight() - win.mouseY()));
	m_Crosshair.setPosition(win.mouseX() * (Settings::Instance().PROJECTION_WIDTH / win.getWidth()) - m_Crosshair.getSize().x / 2.0f,
		((win.getHeight() - win.mouseY()) * (Settings::Instance().PROJECTION_HEIGHT/ win.getHeight())) - m_Crosshair.getSize().y / 2.0f);
	//m_Crosshair.setPosition(win.getMouseWorldPosX(false) - m_Crosshair.getSize().x / 2.0f, win.getMouseWorldPosY(false) - m_Crosshair.getSize().y / 2.0f);
}

void PlayerUI::render(Renderer& renderer)
{
	renderer.render(m_PlayerHealth);
	renderer.render(m_Stamina);
	renderer.render(m_Crosshair);
}
