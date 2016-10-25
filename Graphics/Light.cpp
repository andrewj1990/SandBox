#include "Light.h"

Light::Light()
	: m_Point(glm::vec3(0, 0, 0), glm::vec2(50, 50), TextureManager::get("Textures/Bullet.png")), m_LightRegion(0, 0, 0, 0),
	m_LightBox(glm::vec3(0, 0, 0), glm::vec2(600, 600), TextureManager::get("Textures/light.png")), m_Ray()
{
	m_LightRegion.width = 600;
	m_LightRegion.height = 600;

	m_Ray.setPosition(glm::vec2(m_Point.getPosition().x, m_Point.getPosition().y));

	for (int i = 0; i < 360; i += 7)
	{
		m_Rays.push_back(Ray(glm::vec2(m_Point.getPosition().x, m_Point.getPosition().y), i));
		m_RaySprites.push_back(Renderable(0, 0, 0, 0, 0, 0, 0, 0, glm::vec4(1, 1, 1, 1)));
		//auto& b = m_RaySprites.back();
		//b.setTexture(TextureManager::get("Textures/light_grad.png"));
	}

	//m_RaySprites.push_back(Renderable(500, 500, 500, 500, 600, 600, 600, 550, glm::vec4(1, 1, 1, 1)));
	//m_RaySprites.push_back(Renderable(500, 500, 500, 500, 600, 600, 600, 550, glm::vec4(1, 1, 1, 1)));
	//auto& b2 = m_RaySprites.back();
	//b2.setTexture(TextureManager::get("Textures/light_grad.png"));
}

void Light::update(float timeElapsed)
{
	const Camera& cam = Window::Instance().getCamera();
	float mouseX = Window::Instance().mouseX() + cam.getPosition().x;
	float mouseY = (Window::Instance().getHeight() - Window::Instance().mouseY()) + cam.getPosition().y;
	m_Point.setPosition(glm::vec3(mouseX, mouseY, 0));

	m_LightRegion.x = m_Point.getPosition().x - m_LightRegion.width / 2;
	m_LightRegion.y = m_Point.getPosition().y - m_LightRegion.height / 2;

	m_LightBox.setPosition(m_LightRegion.x, m_LightRegion.y);

	if (Window::Instance().isKeyPressed(GLFW_KEY_Z))
	{
		m_LightBox.setAngle(m_LightBox.getAngle() + 1);
	}
	if (Window::Instance().isKeyPressed(GLFW_KEY_X))
	{
		m_LightBox.setAngle(m_LightBox.getAngle() - 1);
	}

	for (auto& ray : m_Rays)
	{
		ray.setPosition(glm::vec2(m_Point.getPosition().x, m_Point.getPosition().y));
	}

	m_Ray.setPosition(glm::vec2(m_Point.getPosition().x, m_Point.getPosition().y));
	m_Ray.calcAndSetDirection(cam.getPosition().x + Window::Instance().getWidth() / 2, cam.getPosition().y + Window::Instance().getHeight() / 2);
	//m_LightBox.setAngle(m_Ray.getAngle());
	//m_LightBox.setPosition(glm::vec3(m_LightRegion.x, m_LightRegion.y, 0));
}

void Light::update(const std::vector<Renderable*> renderables, float timeElapsed)
{
	m_Ray.intersection(renderables, m_LightRegion);
	//m_LightBox.setSize(glm::vec2(m_Ray.getLength(), 2));
	
	for (auto& ray : m_Rays)
	{
		ray.intersection(renderables, m_LightRegion);
	}

	for (int i = 0; i < m_Rays.size(); i++)
	{
		const auto& endPoint1 = m_Rays[i].getEndPosition();
		const auto& endPoint2 = m_Rays[(i + 1) % m_Rays.size()].getEndPosition();

		auto& quad = m_RaySprites[i];
		quad.setPositions(m_Point.getPosition().x, m_Point.getPosition().y, m_Point.getPosition().x, m_Point.getPosition().y, endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y);
 	}

}

void Light::render(Renderer& renderer)
{
	//renderer.render(m_Point);

	//for (auto& ray : m_Rays)
	//{
	//	ray.render(renderer);
	//}

	//renderer.render(m_RaySprites);
	//for (auto quad : m_RaySprites)
	//{
	//	renderer.submit(quad);
	//}

	renderer.submit(m_LightBox);
}

void Light::renderShadow(Renderer& renderer)
{
	renderer.render(m_RaySprites);
}
