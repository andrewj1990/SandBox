#include "Entity.h"

Entity::Entity()
{
	init();
}

Entity::Entity(float x, float y)
{
	init();
}

Entity::Entity(const glm::vec3& pos, const glm::vec2& size, Texture* texture)
	: m_Sprite(pos, size, texture)
{
	init();
	m_X = pos.x;
	m_Y = pos.y;

	m_CollisionBox = std::shared_ptr<BoundingBox>(new BoundingBox(pos.x, pos.y, size.x, size.y));
}

Entity::Entity(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& colour)
	: m_Sprite(pos, size, colour)
{
	init();
	m_X = pos.x;
	m_Y = pos.y;

	m_CollisionBox = std::shared_ptr<BoundingBox>(new BoundingBox(pos.x, pos.y, size.x, size.y));
}

Entity::~Entity()
{
}

void Entity::init(const glm::vec3& pos, const glm::vec2& size, Texture* texture)
{
	m_Sprite = Sprite(pos, size, texture);

	init();
	m_X = pos.x;
	m_Y = pos.y;
	m_CollisionBox = std::shared_ptr<BoundingBox>(new BoundingBox(pos.x, pos.y, size.x, size.y));
}

void Entity::update(float timeElapsed)
{
}

void Entity::submit(Renderer& renderer)
{
	m_Sprite.submit(renderer);
}

void Entity::render(Renderer& renderer)
{
	//renderer.render(*this);
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

void Entity::init()
{
	m_X = 0;
	m_Y = 0;
	m_Dx = 0.0f;
	m_Dy = 0.0f;

	m_Destroy = false;
	m_CumulativeTime = 0.0f;
}
