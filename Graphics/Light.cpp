#include "Light.h"

Light::Light()
	: m_Point(glm::vec3(0, 0, 0), glm::vec2(50, 50), TextureManager::get("Textures/Bullet.png")), m_LightRegion(0, 0, 0, 0),
	m_LightBox(glm::vec3(0, 0, 0), glm::vec2(600, 200), TextureManager::get("Textures/box.png"))
{
	m_LightRegion.width = 600;
	m_LightRegion.height = 200;

}

void Light::update(float timeElapsed)
{
	const Camera& cam = Window::Instance().getCamera();
	float mouseX = Window::Instance().mouseX() + cam.getPosition().x;
	float mouseY = (Window::Instance().getHeight() - Window::Instance().mouseY()) + cam.getPosition().y;
	m_Point.setPosition(glm::vec3(mouseX, mouseY, 0));

	m_LightRegion.x = m_Point.getPosition().x - m_LightRegion.width / 2;
	m_LightRegion.y = m_Point.getPosition().y - m_LightRegion.height / 2;

	m_LightBox.setPosition(glm::vec3(m_LightRegion.x, m_LightRegion.y, 0));
}

void Light::render(Renderer& renderer)
{
	renderer.render(m_Point);
	renderer.render(m_LightBox);
}
