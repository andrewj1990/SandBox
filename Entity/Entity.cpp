#include "Entity.h"

Entity::Entity()
	: Sprite()
{
	init();
}

Entity::Entity(float x, float y)
	: Sprite(glm::vec3(x, y, 0), glm::vec2(0))
{
	init();
}

Entity::Entity(const glm::vec3& pos, const glm::vec2& size, Texture* texture)
	: Sprite(pos, size, texture)
{
	init();

	m_CollisionBox = std::shared_ptr<BoundingBox>(new BoundingBox(pos.x, pos.y, size.x, size.y));
}

Entity::Entity(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& colour)
	: Sprite(pos, size, colour)
{
	init();

	m_CollisionBox = std::shared_ptr<BoundingBox>(new BoundingBox(pos.x, pos.y, size.x, size.y));
}

Entity::~Entity()
{
}

void Entity::init(const glm::vec3& pos, const glm::vec2& size, Texture* texture)
{
	init();
	m_CollisionBox = std::shared_ptr<BoundingBox>(new BoundingBox(pos.x, pos.y, size.x, size.y));
}

void Entity::update(float timeElapsed)
{
}

void Entity::submit(Renderer& renderer)
{
	renderer.submit(*this);
}

void Entity::render(Renderer& renderer)
{
	renderer.render(*this);
}

bool Entity::collide(const Entity& entity) const
{
	float x = getX();
	float y = getY();
	float w = getWidth();
	float h = getHeight();

	float ex = entity.getX();
	float ey = entity.getY();
	float ew = entity.getWidth();
	float eh = entity.getHeight();

	if (Utils::quadCollision(x, y, w, h, ex, ey, ew, eh))
	{
		return true;
	}

	return false;
}

bool Entity::collide(const Renderable& renderable) const
{
	float x = getX();
	float y = getY();
	float w = getWidth();
	float h = getHeight();

	float ex = renderable.getPosition().x;
	float ey = renderable.getPosition().y;
	float ew = renderable.getSize().x;
	float eh = renderable.getSize().y;

	if (Utils::quadCollision(x, y, w, h, ex, ey, ew, eh))
	{
		return true;
	}

	return false;
}

bool Entity::collide(const BoundingBox& bbox) const
{
	float x = getX();
	float y = getY();
	float w = getWidth();
	float h = getHeight();

	float ex = bbox.x;
	float ey = bbox.y;
	float ew = bbox.width;
	float eh = bbox.height;

	if (Utils::quadCollision(x, y, w, h, ex, ey, ew, eh))
	{
		return true;
	}

	return false;
}

float Entity::getAngle(const Entity& entity)
{
	float x = getCenterX();
	float y = getCenterY();
	float ex = entity.getCenterX();
	float ey = entity.getCenterY();

	float dx = ex - x;
	float dy = ey - y;

	return std::atan2f(dy, dx);
}

float Entity::calcDistanceSqrd(const Entity& entity) const
{
	float x1 = getCenterX();
	float y1 = getCenterY();
	float x2 = entity.getCenterX();
	float y2 = entity.getCenterY();

	float dx = x2 - x1;
	float dy = y2 - y1;

	return (dx * dx + dy * dy);
}

void Entity::init()
{
	m_Dx = 0.0f;
	m_Dy = 0.0f;

	m_Destroy = false;
	m_CumulativeTime = 0.0f;
}
