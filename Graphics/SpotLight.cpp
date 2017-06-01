#include "SpotLight.h"

SpotLight::SpotLight()
	: position_(0, 0), light_region_(0, 0, 0, 0)
{
	light_region_.width = 400;		// diameter
	light_region_.height = light_region_.width;
}

void SpotLight::update(float x, float y, float timeElapsed)
{
	std::vector<Entity*> sprites;
	ObjectManager::ObjectsQT->retrieve(sprites, light_region_);

	position_.x = x;
	position_.y = y;

	light_region_.x = position_.x - light_region_.width / 2;
	light_region_.y = position_.y - light_region_.width / 2;

	quads_.clear();
	std::vector<Ray> rays;

	// add rays for each sprite
	for (const auto& sprite : sprites)
	{
		int sx = sprite->getOccluder().x;
		int sy = sprite->getOccluder().y;
		int sw = sprite->getOccluder().width;
		int sh = sprite->getOccluder().height;

		if (!Utils::inRange(position_.x, position_.y, sx, sy, (light_region_.width / 2.0f))) continue;

		rays.push_back(Ray(glm::vec2(position_.x, position_.y), Utils::calcAngleRad(position_.x, position_.y, sx, sy) + 0.00001f));
		rays.push_back(Ray(glm::vec2(position_.x, position_.y), Utils::calcAngleRad(position_.x, position_.y, sx + sw, sy) + 0.00001f));
		rays.push_back(Ray(glm::vec2(position_.x, position_.y), Utils::calcAngleRad(position_.x, position_.y, sx + sw, sy + sh) + 0.00001f));
		rays.push_back(Ray(glm::vec2(position_.x, position_.y), Utils::calcAngleRad(position_.x, position_.y, sx, sy + sh) + 0.00001f));

		rays.push_back(Ray(glm::vec2(position_.x, position_.y), Utils::calcAngleRad(position_.x, position_.y, sx, sy) - 0.00001f));
		rays.push_back(Ray(glm::vec2(position_.x, position_.y), Utils::calcAngleRad(position_.x, position_.y, sx + sw, sy) - 0.00001f));
		rays.push_back(Ray(glm::vec2(position_.x, position_.y), Utils::calcAngleRad(position_.x, position_.y, sx + sw, sy + sh) - 0.00001f));
		rays.push_back(Ray(glm::vec2(position_.x, position_.y), Utils::calcAngleRad(position_.x, position_.y, sx, sy + sh) - 0.00001f));
	}

	// add bounding box rays
	int sx = light_region_.x;
	int sy = light_region_.y;
	int sw = light_region_.width;
	int sh = light_region_.width;

	rays.push_back(Ray(glm::vec2(position_.x, position_.y), Utils::calcAngleRad(position_.x, position_.y, sx, sy) + 0.00001f));
	rays.push_back(Ray(glm::vec2(position_.x, position_.y), Utils::calcAngleRad(position_.x, position_.y, sx + sw, sy) + 0.00001f));
	rays.push_back(Ray(glm::vec2(position_.x, position_.y), Utils::calcAngleRad(position_.x, position_.y, sx + sw, sy + sh) + 0.00001f));
	rays.push_back(Ray(glm::vec2(position_.x, position_.y), Utils::calcAngleRad(position_.x, position_.y, sx, sy + sh) + 0.00001f));

	rays.push_back(Ray(glm::vec2(position_.x, position_.y), Utils::calcAngleRad(position_.x, position_.y, sx, sy) - 0.00001f));
	rays.push_back(Ray(glm::vec2(position_.x, position_.y), Utils::calcAngleRad(position_.x, position_.y, sx + sw, sy) - 0.00001f));
	rays.push_back(Ray(glm::vec2(position_.x, position_.y), Utils::calcAngleRad(position_.x, position_.y, sx + sw, sy + sh) - 0.00001f));
	rays.push_back(Ray(glm::vec2(position_.x, position_.y), Utils::calcAngleRad(position_.x, position_.y, sx, sy + sh) - 0.00001f));


	for (auto& ray : rays)
	{
		ray.findIntersections(sprites, light_region_, light_region_.width / 2);
	}

	std::sort(rays.begin(), rays.end(), [](const auto& a, const auto& b) { return a.getAngle() < b.getAngle(); });

	for (int i = 0; i < rays.size(); i++)
	{
		const auto& endPoint1 = rays[i].getEndPoint();
		const auto& endPoint2 = rays[(i + 1) % rays.size()].getEndPoint();

		quads_.push_back(Renderable(position_.x, position_.y, position_.x, position_.y, endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y, glm::vec4(1, 1, 1, 1)));
		quads_.back().setLightPosition(position_.x, position_.y, light_region_.width / 2);
	}
}

void SpotLight::render(Renderer& renderer)
{
	renderer.begin();

	for (auto& quad : quads_)
	{
		quad.submit(renderer);
	}

	renderer.end();
	renderer.flush();
}
