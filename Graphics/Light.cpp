#include "Light.h"

Light::Light()
	: m_Point(glm::vec3(0, 0, 0), glm::vec2(50, 50), TextureManager::get("Textures/Bullet.png")), m_LightRegion(0, 0, 0, 0),
	m_LightBox(glm::vec3(0, 0, 0), glm::vec2(20, 20), TextureManager::get("Textures/light.png")), m_Ray()
{
	m_LightRegion.width = 200;
	m_LightRegion.height = 200;

	m_Ray.setPosition(glm::vec2(m_Point.getPosition().x, m_Point.getPosition().y));

	for (int i = 0; i < 360; i += 7)
	{
		m_Rays.push_back(LineRay(glm::vec2(m_Point.getPosition().x, m_Point.getPosition().y), i));
		m_RaySprites.push_back(Renderable(0, 0, 0, 0, 0, 0, 0, 0, glm::vec4(1, 1, 1, 1)));
	}
}

Light::Light(const Light& other)
	: m_Point(other.m_Point), m_LightBox(other.m_LightBox), m_Ray(other.m_Ray), m_LightRegion(other.m_LightRegion), m_Rays(other.m_Rays), m_RaySprites(other.m_RaySprites)
{
}

void Light::update(float x, float y, float timeElapsed)
{
	const Camera& cam = Window::Instance().getCamera();
	m_Point.setPosition(glm::vec3(x, y, 0));

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

	//m_Ray.setPosition(glm::vec2(m_Point.getPosition().x, m_Point.getPosition().y));
	//m_Ray.calcAndSetDirection(cam.getPosition().x + Window::Instance().getWidth() / 2, cam.getPosition().y + Window::Instance().getHeight() / 2);
}

void Light::update(const std::vector<std::shared_ptr<Sprite>>& renderables, float timeElapsed)
{
	//m_Ray.intersection(renderables, m_LightRegion);
	m_LightBox.setSize(glm::vec2(m_Ray.getLength(), 2));
	
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
		quad.setLightPosition(m_Point.getPosition().x, m_Point.getPosition().y);
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

	//renderer.submit(m_LightBox);
}

void Light::renderShadow(Renderer& renderer)
{
	for (auto quad : m_RaySprites)
	{
		renderer.submit(quad);
	}
}
