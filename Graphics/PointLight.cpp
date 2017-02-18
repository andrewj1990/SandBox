#include "PointLight.h"

PointLight::PointLight()
	: m_Position(0, 0), m_LightRegion(0, 0, 0, 0), m_LightBox(glm::vec3(0, 0, 0), glm::vec2(200, 200), TextureManager::get("Textures/light.png"))
{
	m_LightRegion.width = 200;
	m_LightRegion.height = 200;

	for (int i = 0; i < 360; i += 7)
	{
		m_Rays.push_back(Ray(glm::vec2(m_Position.x, m_Position.y), i));
		m_RaySprites.push_back(Renderable(0, 0, 0, 0, 0, 0, 0, 0, glm::vec4(1, 1, 1, 1)));
	}
}

PointLight::PointLight(const PointLight& other)
	: m_Position(other.m_Position), m_LightBox(other.m_LightBox), m_LightRegion(other.m_LightRegion), m_Rays(other.m_Rays), m_RaySprites(other.m_RaySprites)
{
}

void PointLight::update(float x, float y, const std::vector<std::shared_ptr<Sprite>>& renderables, float timeElapsed)
{
	const Camera& cam = Window::Instance().getCamera();
	m_Position.x = x;
	m_Position.y = y;

	m_LightRegion.x = m_Position.x - m_LightRegion.width / 2;
	m_LightRegion.y = m_Position.y - m_LightRegion.height / 2;

	m_LightBox.setPosition(m_LightRegion.x, m_LightRegion.y);

	for (auto& ray : m_Rays)
	{
		ray.setPosition(glm::vec2(m_Position.x, m_Position.y));
	}
	
	for (auto& ray : m_Rays)
	{
		ray.intersect(renderables, m_LightRegion);
	}

	for (int i = 0; i < m_Rays.size(); i++)
	{
		const auto& endPoint1 = m_Rays[i].getEndPoint();
		const auto& endPoint2 = m_Rays[(i + 1) % m_Rays.size()].getEndPoint();

		auto& quad = m_RaySprites[i];
		quad.setPositions(m_Position.x, m_Position.y, m_Position.x, m_Position.y, endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y);
		quad.setLightPosition(m_Position.x, m_Position.y);
	}
}

void PointLight::render(Renderer& renderer)
{
	for (auto quad : m_RaySprites)
	{
		renderer.submit(quad);
	}
}
