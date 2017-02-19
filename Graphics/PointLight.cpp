#include "PointLight.h"

PointLight::PointLight()
	: m_Position(0, 0), m_LightRegion(0, 0, 0, 0)
{
	m_LightRegion.width = 400;		// diameter
	m_LightRegion.height = m_LightRegion.width;
}

PointLight::PointLight(const PointLight& other)
	: m_Position(other.m_Position), m_LightRegion(other.m_LightRegion)
{
	for (const auto& quads : other.m_LightQuads)
	{
		m_LightQuads.push_back(std::unique_ptr<Renderable>(new Renderable(other.m_Position.x, other.m_Position.y, other.m_Position.x, other.m_Position.y,
			quads->getPositions()[2].x, quads->getPositions()[2].y, quads->getPositions()[3].x, quads->getPositions()[3].y, glm::vec4(1, 1, 1, 1))));
		m_LightQuads.back()->setLightPosition(m_Position.x, m_Position.y, m_LightRegion.width / 2);
	}
}

void PointLight::createRays(const std::vector<std::shared_ptr<Sprite>>& sprites)
{
	m_Rays = std::vector<std::unique_ptr<Ray>>();

	// add rays for each sprite
	for (const auto& sprite : sprites)
	{
		int sx = sprite->getCollisionBox()->x;
		int sy = sprite->getCollisionBox()->y;
		int sw = sprite->getCollisionBox()->width;
		int sh = sprite->getCollisionBox()->height;

		if (!Utils::inRange(m_Position.x, m_Position.y, sx, sy, (m_LightRegion.width / 2.0f))) continue;

		m_Rays.push_back(std::unique_ptr<Ray>(new Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx     , sy	 ) + 0.00001f)));
		m_Rays.push_back(std::unique_ptr<Ray>(new Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx + sw, sy	 ) + 0.00001f)));
		m_Rays.push_back(std::unique_ptr<Ray>(new Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx + sw, sy + sh) + 0.00001f)));
		m_Rays.push_back(std::unique_ptr<Ray>(new Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx     , sy + sh) + 0.00001f)));

		m_Rays.push_back(std::unique_ptr<Ray>(new Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx     , sy	 ) - 0.00001f)));
		m_Rays.push_back(std::unique_ptr<Ray>(new Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx + sw, sy	 ) - 0.00001f)));
		m_Rays.push_back(std::unique_ptr<Ray>(new Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx + sw, sy + sh) - 0.00001f)));
		m_Rays.push_back(std::unique_ptr<Ray>(new Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx     , sy + sh) - 0.00001f)));
	}
	
	// add bounding box rays
	int sx = m_LightRegion.x;
	int sy = m_LightRegion.y;
	int sw = m_LightRegion.width;
	int sh = m_LightRegion.width;

	m_Rays.push_back(std::unique_ptr<Ray>(new Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx     , sy	 ) + 0.00001f)));
	m_Rays.push_back(std::unique_ptr<Ray>(new Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx + sw, sy	 ) + 0.00001f)));
	m_Rays.push_back(std::unique_ptr<Ray>(new Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx + sw, sy + sh) + 0.00001f)));
	m_Rays.push_back(std::unique_ptr<Ray>(new Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx     , sy + sh) + 0.00001f)));

	m_Rays.push_back(std::unique_ptr<Ray>(new Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx     , sy	 ) - 0.00001f)));
	m_Rays.push_back(std::unique_ptr<Ray>(new Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx + sw, sy	 ) - 0.00001f)));
	m_Rays.push_back(std::unique_ptr<Ray>(new Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx + sw, sy + sh) - 0.00001f)));
	m_Rays.push_back(std::unique_ptr<Ray>(new Ray(glm::vec2(m_Position.x, m_Position.y), Utils::calcAngleRad(m_Position.x, m_Position.y, sx     , sy + sh) - 0.00001f)));
	
}

void PointLight::update(float x, float y, const std::unique_ptr<QTree<Sprite>>& objectsQT, float timeElapsed)
{
	std::vector<std::shared_ptr<Sprite>> sprites;
	objectsQT->retrieve(sprites, m_LightRegion);

	m_Position.x = x;
	m_Position.y = y;

	m_LightRegion.x = m_Position.x - m_LightRegion.width / 2;
	m_LightRegion.y = m_Position.y - m_LightRegion.width / 2;

	m_LightQuads = std::vector<std::unique_ptr<Renderable>>();

	createRays(sprites);

	for (auto& ray : m_Rays)
	{
		ray->findIntersections(sprites, m_LightRegion, m_LightRegion.width / 2);
	}

	std::sort(m_Rays.begin(), m_Rays.end(), [](const auto& a, const auto& b) { return a->getAngle() < b->getAngle(); });

	for (int i = 0; i < m_Rays.size(); i++)
	{
		const auto& endPoint1 = m_Rays[i]->getEndPoint();
		const auto& endPoint2 = m_Rays[(i + 1) % m_Rays.size()]->getEndPoint();

		m_LightQuads.push_back(std::unique_ptr<Renderable>(new Renderable(m_Position.x, m_Position.y, m_Position.x, m_Position.y, endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y, glm::vec4(1, 1, 1, 1))));
		m_LightQuads.back()->setLightPosition(m_Position.x, m_Position.y, m_LightRegion.width / 2);
	}
}

void PointLight::render(Renderer& renderer)
{
	for (auto& quad : m_LightQuads)
	{
		renderer.submit(*quad);
	}
}
