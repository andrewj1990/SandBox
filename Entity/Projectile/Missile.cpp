#include "Missile.h"

Missile::Missile(float x, float y, Entity* target)
	: Entity(glm::vec3(x, y, 0), glm::vec2(16), TextureManager::get("Textures/Missile.png")), m_Target(target), m_Transform()
{
	m_Angle = glm::radians(Utils::random(0.0f, 360.0f));
	//m_Angle = Utils::calcAngleRad(x, y, Window::Instance().getMouseWorldPosX(), Window::Instance().getMouseWorldPosY());
	m_TargetTime = Utils::random(0.4f, 1.0f);

	m_Velocity = 600.0f;
	m_AngleDelta = glm::radians(150.0f);
}

void Missile::update(float timeElapsed)
{
	if (m_Destroy)
	{
		return;
	}

	// target is dead so retarget
	if (m_Target == nullptr || m_Target->shouldDestroy())
	{
		retarget();
	}

	m_CumulativeTime += timeElapsed;
	angleDelta(timeElapsed);

	if (collide(m_Target->getCollisionBox()))
	{
		if (!m_Target->shouldDestroy())
		{
			m_Target->setAngle(m_Angle);
			m_Target->damage(1);
		}
		m_Destroy = true;
		return;
	}

	float colour = Utils::random(0.8f, 1.0f);
	for (int i = 0; i < 3; i++)
	{
		ParticleManager::instance().add(Particle(getX(), getY(), 5, glm::vec4(glm::vec3(colour, colour, 0.0f), 1), glm::degrees(m_Angle + 180.0f), false, 5.0f));
	}

	m_Transform = glm::mat4();
	m_Transform = glm::translate(m_Transform, glm::vec3(getCenterX(), getCenterY(), 0));
	m_Transform = glm::rotate(m_Transform, getAngle(), glm::vec3(0, 0, 1));
	m_Transform = glm::translate(m_Transform, glm::vec3(-getCenterX(), -getCenterY(), 0));

	addDirection(m_Velocity * std::cosf(m_Angle) * timeElapsed, m_Velocity * std::sinf(m_Angle) * timeElapsed);
}

void Missile::submit(Renderer & renderer)
{
	renderer.push(m_Transform);
	renderer.submit(*this);
	renderer.pop();
}

void Missile::retarget()
{
	std::vector<Entity*> objects;

	ObjectManager::MobQT->retrieve(objects, BoundingBox(getCenterX() - 500, getCenterY() - 500, 1000.0f, 1000.0f));

	if (!objects.empty())
	{
		int randomVal = Utils::random(0, objects.size());
		m_Target = objects[randomVal];
	}
}

void Missile::angleDelta(float timeElapsed)
{
	float angleOffset = glm::radians(Utils::random(-10.0f, 10.0f));

	float angleToTarget = Utils::calcAngleDeg(getCenterX(), getCenterY(), m_Target->getCenterX(), m_Target->getCenterY());
	float currAngle = glm::degrees(m_Angle);

	float offset = Utils::random(-20.0f, 20.0f);
	float newAngle = Utils::random(currAngle + offset, angleToTarget);

	if (m_CumulativeTime > m_TargetTime)
	{
		m_Angle = glm::radians(newAngle + offset);
	}
	else
	{
		m_Angle += angleOffset;
	}

}
