#include "Entity.h"

Entity::Entity()
	: m_X(0), m_Y(0), m_Dx(0.0f), m_Dy(0.0f), m_Destroy(false), m_CumulativeTime(0.0f)
{
}

Entity::Entity(float x, float y)
	: m_X(x), m_Y(y), m_Dx(0.0f), m_Dy(0.0f), m_Destroy(false), m_CumulativeTime(0.0f)
{
}

Entity::~Entity()
{
}

void Entity::update(float timeElapsed)
{
}

void Entity::render(Renderer& renderer)
{
	renderer.render(m_Sprite);
}