#include "Particle.h"

Particle::Particle(float x, float y, float size, float angleDeg, bool applyTransform)
	: Entity(glm::vec3(x, y, 1000.0f), glm::vec2(size, size), glm::vec4(Utils::random(0.7f, 1.0f), Utils::random(0.0f, 1.0f), 0, rand() % 1000 / 1000.0f)), m_Transform()
{
	m_FadeSpeed = 0.2f;
	m_ApplyTransform = applyTransform;
	float angleOffset = Utils::random(-20.0f, 20.0f);
	setLightPosition(getCenterX(), getCenterY(), m_Size.x);

	float vel = Utils::random(20.0f, 100.0f);
	float a = glm::radians(angleDeg + angleOffset);

	m_Dx = std::cosf(a) * vel;
	m_Dy = std::sinf(a) * vel;

	m_Duration = Utils::random(0.2f, 1.0f);
	m_Duration = 2.0f;

	m_Angle = glm::radians(Utils::random(0.0f, 360.0f));
	m_RotationSpeed = glm::radians(Utils::random(-5.0f, 5.0f));
	m_Dz = 0;
}

Particle::Particle(float x, float y, float size, const glm::vec4& colour, float angleDeg, bool applyTransform, float fadeSpeed)
	: Entity(glm::vec3(x, y, 1000.0f), glm::vec2(size), colour), m_Transform()
{
	m_FadeSpeed = fadeSpeed;
	m_ApplyTransform = applyTransform;
	float angleOffset = Utils::random(-20.0f, 20.0f);
	float velSlow = angleOffset / 20.0f;
	velSlow = 1.0f - std::abs(velSlow);
	setLightPosition(getCenterX(), getCenterY(), m_Size.x);

	float vel = Utils::random(200.0f, 800.0f) * velSlow;
	float a = glm::radians(angleDeg + angleOffset);

	m_Dx = std::cosf(a) * vel;
	m_Dy = std::sinf(a) * vel;

	m_Duration = Utils::random(100.2f, 104.0f);
	m_Duration = 2.0f;

	m_Angle = glm::radians(Utils::random(0.0f, 360.0f));
	m_RotationSpeed = glm::radians(Utils::random(-5.0f, 5.0f));
	m_Dz = 0;
}

Particle::Particle(float x, float y, float size, Texture* texture, bool applyTransform)
	: Entity(glm::vec3(x, y, 1000.0f), glm::vec2(size, size), texture), m_Transform()
{
	m_FadeSpeed = 0.2f;
	m_ApplyTransform = applyTransform;
	//int w = Utils::random(2, texture->getWidth());
	//int h = Utils::random(2, texture->getHeight());
	//int ux = Utils::random(0, texture->getWidth() / w);
	//int uy = Utils::random(0, texture->getHeight() / h);
	//setUV(ux, uy, w, h);
	
	float angleOffset = Utils::random(-20.0f, 20.0f);

	float angleDeg = Utils::random(0, 360);

	//float vel = Utils::random(20.0f, 100.0f);
	float vel = Utils::random(100.0f, 250.0f);
	float a = glm::radians(angleDeg);

	m_Dx = std::cosf(a) * vel;
	m_Dy = std::sinf(a) * vel;

	//m_Duration = Utils::random(0.2f, 1.0f);
	//m_Duration = Utils::random(10.2f, 14.0f);
	m_Duration = 2.0f;

	m_Angle = glm::radians(Utils::random(0.0f, 360.0f));
	m_RotationSpeed = glm::radians(Utils::random(-5.0f, 5.0f));

	m_Dz = 100;
}


Particle::Particle(float x, float y, float size, float angleDeg, Texture* texture, bool applyTransform)
	: Entity(glm::vec3(x, y, 1000.0f), glm::vec2(size, size), texture), m_Transform()
{
	m_FadeSpeed = 0.2f;
	m_ApplyTransform = applyTransform;

	float vel = Utils::random(100.0f, 250.0f);
	float a = glm::radians(angleDeg);

	m_Dx = std::cosf(a) * vel;
	m_Dy = std::sinf(a) * vel;

	//m_Duration = Utils::random(0.2f, 1.0f);
	//m_Duration = Utils::random(10.2f, 14.0f);
	m_Duration = 2.0f;

	m_Angle = glm::radians(Utils::random(0.0f, 360.0f));
	m_RotationSpeed = glm::radians(Utils::random(-5.0f, 5.0f));

	m_Dz = 100;
}


void Particle::update(float timeElapsed)
{
	if (m_ApplyTransform)
	{
		m_Transform = glm::mat4();
		m_Transform = glm::translate(m_Transform, glm::vec3(getCenterX(), getCenterY(), 0));
		m_Transform = glm::rotate(m_Transform, getAngle(), glm::vec3(0, 0, 1));
		m_Transform = glm::translate(m_Transform, glm::vec3(-getCenterX(), -getCenterY(), 0));
	}

	addDirection(m_Dx * timeElapsed, (m_Dy - m_Dz) * timeElapsed);
	//setLightPosition(getCenterX(), getCenterY(), m_Size.x);
	
	m_CumulativeTime += timeElapsed;

	if (m_CumulativeTime > 0.01)
	{
		m_Dx *= 0.94f;
		m_Dy *= 0.94f;
		m_Dz -= 1.0f;
		m_RotationSpeed *= 0.99f;
		m_CumulativeTime -= 0.01f;

		if (m_Dz < 0)
		{
			m_Dz *= -0.98f;
		}

		//fade(m_FadeSpeed * timeElapsed);

		m_Angle += m_RotationSpeed;
	}

	m_Duration -= timeElapsed;

	if (m_Duration <= 0 /*|| getColour().w <= 0.01f*/)
	{
		m_Destroy = true;
	}

}

void Particle::submit(Renderer& renderer)
{
	if (m_ApplyTransform) renderer.push(m_Transform);
	renderer.submit(*this);
	if (m_ApplyTransform) renderer.pop();
}
