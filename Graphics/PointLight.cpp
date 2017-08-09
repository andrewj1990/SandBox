#include "PointLight.h"

PointLight::PointLight()
	: m_Position(0, 0), m_LightRegion(0, 0, 0, 0)
{
	m_LightRegion.width = 900;		// diameter
	m_LightRegion.height = m_LightRegion.width;
}

PointLight::PointLight(const PointLight& other)
	: m_Position(other.m_Position), m_LightRegion(other.m_LightRegion), m_Quads(other.m_Quads)
{
}

void PointLight::update(float x, float y, float timeElapsed)
{
	std::vector<Entity*> sprites;
	ObjectManager::ObjectsQT->retrieve(sprites, m_LightRegion);

	m_Position.x = x;
	m_Position.y = y;

	m_LightRegion.x = m_Position.x - m_LightRegion.width / 2;
	m_LightRegion.y = m_Position.y - m_LightRegion.width / 2;

	m_Quads.clear();
	std::vector<Ray> rays;

	// add rays for each sprite
	for (const auto& sprite : sprites)
	{
		int sx = sprite->getOccluder().x;
		int sy = sprite->getOccluder().y;
		int sw = sprite->getOccluder().width;
		int sh = sprite->getOccluder().height;

		if (!Utils::inRange(m_Position.x, m_Position.y, sx, sy, (m_LightRegion.width / 2.0f))) continue;

		rays.push_back(Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx, sy) + 0.00001f));
		rays.push_back(Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx + sw, sy) + 0.00001f));
		rays.push_back(Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx + sw, sy + sh) + 0.00001f));
		rays.push_back(Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx, sy + sh) + 0.00001f));

		rays.push_back(Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx, sy) - 0.00001f));
		rays.push_back(Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx + sw, sy) - 0.00001f));
		rays.push_back(Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx + sw, sy + sh) - 0.00001f));
		rays.push_back(Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx, sy + sh) - 0.00001f));
	}

	// add bounding box rays
	int sx = m_LightRegion.x;
	int sy = m_LightRegion.y;
	int sw = m_LightRegion.width;
	int sh = m_LightRegion.width;

	rays.push_back(Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx, sy) + 0.00001f));
	rays.push_back(Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx + sw, sy) + 0.00001f));
	rays.push_back(Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx + sw, sy + sh) + 0.00001f));
	rays.push_back(Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx, sy + sh) + 0.00001f));

	rays.push_back(Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx, sy) - 0.00001f));
	rays.push_back(Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx + sw, sy) - 0.00001f));
	rays.push_back(Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx + sw, sy + sh) - 0.00001f));
	rays.push_back(Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx, sy + sh) - 0.00001f));


	for (auto& ray : rays)
	{
		ray.findIntersections(sprites, m_LightRegion, m_LightRegion.width / 2);
	}

	std::sort(rays.begin(), rays.end(), [](const auto& a, const auto& b) { return a.getAngle() < b.getAngle(); });

	for (int i = 0; i < rays.size(); i++)
	{
		const auto& endPoint1 = rays[i].getEndPoint();
		const auto& endPoint2 = rays[(i + 1) % rays.size()].getEndPoint();

		m_Quads.push_back(Renderable(m_Position.x, m_Position.y, m_Position.x, m_Position.y, endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y, glm::vec4(1, 1, 1, 1)));
		m_Quads.back().setLightPosition(m_Position.x, m_Position.y, m_LightRegion.width / 2);
	}
}

void PointLight::render(Renderer& renderer)
{
	for (auto& quad : m_Quads)
	{
		renderer.submit(quad);
	}
}
