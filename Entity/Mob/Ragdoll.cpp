#include "Ragdoll.h"

Ragdoll::Ragdoll(float x, float y, float size, float angleDeg, Texture* texture)
	: Entity(glm::vec3(x, y, 0), glm::vec2(size), texture)
{
	m_KnockbackDistance = Utils::random(100.0f, 500.0f);

	float angleRad = glm::radians(angleDeg);
	m_Dx = m_KnockbackDistance * std::cosf(angleRad);
	m_Dy = m_KnockbackDistance * std::sinf(angleRad);

	m_Angle = angleRad;
}

void Ragdoll::update(float timeElapsed)
{
	m_CumulativeTime += timeElapsed;

	if (getColour().w <= 0.01f)
	{
		m_Destroy = true;
		return;
	}

	if (m_CumulativeTime > 0.01)
	{
		m_Dx *= 0.94f;
		m_Dy *= 0.94f;
		m_CumulativeTime -= 0.01f;

		fade(0.2f * timeElapsed);
	}

	if (std::abs(m_Dx) < 10.0f && std::abs(m_Dy) < 10.0f)
	{
		return;
	}

	if (std::abs(m_Dx) < 100.0f && std::abs(m_Dy) < 100.0f)
	{

		for (int i = 0; i < 50; i++)
		{
			ParticleManager::instance().addReady(Particle(getCenterX(), getCenterY(), Utils::random(1, 10), glm::vec4(Utils::random(0.8f, 1.0f), 0, 0, Utils::random(0.4f, 1.0f)), glm::degrees(getAngle()) + 180.0f + Utils::random(0.0f, 360.0f)));
		}

		m_KnockbackDistance *= 0.5f;
		m_Dx = m_KnockbackDistance * std::cosf(m_Angle);
		m_Dy = m_KnockbackDistance * std::sinf(m_Angle);
	}
	else
	{
		for (int i = 0; i < 5; i++)
		{
			ParticleManager::instance().addReady(Particle(getCenterX(), getCenterY(), Utils::random(1, 10), glm::vec4(Utils::random(0.8f, 1.0f), 0, 0, Utils::random(0.4f, 1.0f)), glm::degrees(getAngle()) + 180.0f + Utils::random(-10.0f, 10.0f)));
		}
	}

	addDirection(m_Dx * timeElapsed, m_Dy * timeElapsed);
}

void Ragdoll::submit(Renderer& renderer)
{
	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(getCenterX(), getCenterY(), 0));
	transform = glm::rotate(transform, getAngle() + glm::radians(180.0f), glm::vec3(0, 0, 1));
	transform = glm::translate(transform, glm::vec3(-getCenterX(), -getCenterY(), 0));
	renderer.push(transform);
	renderer.submit(*this);
	renderer.pop();
}
