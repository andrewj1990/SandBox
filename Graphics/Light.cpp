#include "Light.h"

Light::Light()
	: m_Point(glm::vec3(0, 0, 0), glm::vec2(50, 50), TextureManager::get("Textures/Bullet.png")), m_LightRegion(0, 0, 0, 0),
	m_LightBox(glm::vec3(0, 0, 0), glm::vec2(600, 600), glm::vec4(0,1,0,1)), m_Ray()
{
	m_LightRegion.width = 400;
	m_LightRegion.height = 400;

	m_Ray.setPosition(glm::vec2(m_Point.getPosition().x, m_Point.getPosition().y));
}

void Light::update(float timeElapsed)
{
	const Camera& cam = Window::Instance().getCamera();
	float mouseX = Window::Instance().mouseX() + cam.getPosition().x;
	float mouseY = (Window::Instance().getHeight() - Window::Instance().mouseY()) + cam.getPosition().y;
	m_Point.setPosition(glm::vec3(mouseX, mouseY, 0));

	m_LightRegion.x = m_Point.getPosition().x - m_LightRegion.width / 2;
	m_LightRegion.y = m_Point.getPosition().y - m_LightRegion.height / 2;

	m_LightBox.setPosition(m_Point.getPosition().x, m_Point.getPosition().y);

	if (Window::Instance().isKeyPressed(GLFW_KEY_Z))
	{
		m_LightBox.setAngle(m_LightBox.getAngle() + 1);
	}
	if (Window::Instance().isKeyPressed(GLFW_KEY_X))
	{
		m_LightBox.setAngle(m_LightBox.getAngle() - 1);
	}

	m_Ray.setPosition(glm::vec2(m_Point.getPosition().x, m_Point.getPosition().y));
	m_Ray.calcAndSetDirection(cam.getPosition().x + Window::Instance().getWidth() / 2, cam.getPosition().y + Window::Instance().getHeight() / 2);
	m_LightBox.setAngle(m_Ray.getAngle());
	//m_LightBox.setPosition(glm::vec3(m_LightRegion.x, m_LightRegion.y, 0));
}

void Light::update(const std::vector<Renderable*> renderables, float timeElapsed)
{
	m_Ray.intersection(renderables);
	m_LightBox.setSize(glm::vec2(m_Ray.getLength(), 2));
}

void Light::render(Renderer& renderer)
{
	renderer.render(m_Point);

	glm::mat4 m;
	m = glm::translate(m, glm::vec3(m_LightBox.getPosition().x, m_LightBox.getPosition().y, 0));
	m = glm::rotate(m, m_LightBox.getAngle(), glm::vec3(0, 0, 1));
	m = glm::translate(m, glm::vec3(-m_LightBox.getPosition().x, -m_LightBox.getPosition().y, 0));
	renderer.push(m);
	renderer.render(m_LightBox);
	renderer.pop();
}
