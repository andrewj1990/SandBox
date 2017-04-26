#include "Particle.h"

Particle::Particle(float x, float y, float size, float angleDeg)
	: Entity(glm::vec3(x, y, 1000.0f), glm::vec2(size, size), glm::vec4(Utils::random(0.7f, 1.0f), Utils::random(0.0f, 1.0f), 0, rand() % 1000 / 1000.0f))
{
	float angleOffset = Utils::random(-20.0f, 20.0f);
	setLightPosition(getCenterX(), getCenterY(), m_Size.x);

	float vel = Utils::random(20.0f, 100.0f);
	float a = glm::radians(angleDeg + angleOffset);

	m_Dx = std::cosf(a) * vel;
	m_Dy = std::sinf(a) * vel;

	m_Duration = Utils::random(0.2f, 1.0f);

	m_Angle = glm::radians(Utils::random(0.0f, 360.0f));
	m_RotationSpeed = glm::radians(Utils::random(-5.0f, 5.0f));
	m_Dz = 0;
}

Particle::Particle(float x, float y, float size, Texture* texture)
	: Entity(glm::vec3(x, y, 1000.0f), glm::vec2(size, size), texture)
{
	int w = Utils::random(2, texture->getWidth());
	int h = Utils::random(2, texture->getHeight());
	int ux = Utils::random(0, texture->getWidth() / w);
	int uy = Utils::random(0, texture->getHeight() / h);
	setUV(ux, uy, w, h);
	
	float angleOffset = Utils::random(-20.0f, 20.0f);

	float angleDeg = Utils::random(0, 360);

	//float vel = Utils::random(20.0f, 100.0f);
	float vel = Utils::random(100.0f, 250.0f);
	float a = glm::radians(angleDeg);

	m_Dx = std::cosf(a) * vel;
	m_Dy = std::sinf(a) * vel;

	//m_Duration = Utils::random(0.2f, 1.0f);
	m_Duration = Utils::random(2.2f, 4.0f);

	m_Angle = glm::radians(Utils::random(0.0f, 360.0f));
	m_RotationSpeed = glm::radians(Utils::random(-5.0f, 5.0f));

	m_Dz = 100;
}


Particle::Particle(float x, float y, float size, float angleDeg, Texture* texture)
	: Entity(glm::vec3(x, y, 1000.0f), glm::vec2(size, size), texture)
{
	float vel = Utils::random(100.0f, 250.0f);
	float a = glm::radians(angleDeg);

	m_Dx = std::cosf(a) * vel;
	m_Dy = std::sinf(a) * vel;

	//m_Duration = Utils::random(0.2f, 1.0f);
	m_Duration = Utils::random(2.2f, 4.0f);

	m_Angle = glm::radians(Utils::random(0.0f, 360.0f));
	m_RotationSpeed = glm::radians(Utils::random(-5.0f, 5.0f));

	m_Dz = 100;
}


void Particle::update(float timeElapsed)
{
	addDirection(m_Dx * timeElapsed, (m_Dy - m_Dz) * timeElapsed);
	//setLightPosition(getCenterX(), getCenterY(), m_Size.x);

	m_Dx *= 0.98f;
	m_Dy *= 0.98f;
	m_Dz -= 1.0f;
	m_RotationSpeed *= 0.99f;

	if (m_Dz < 0)
	{
		m_Dz *= -0.98f;
	}

	//fade(0.03f);

	m_Angle += m_RotationSpeed;

	m_Duration -= timeElapsed;

	if (m_Duration <= 0)
	{
		m_Destroy = true;
	}

}

void Particle::submit(Renderer& renderer)
{
	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(getCenterX(), getCenterY(), 0));
	transform = glm::rotate(transform, getAngle(), glm::vec3(0, 0, 1));
	transform = glm::translate(transform, glm::vec3(-getCenterX(), -getCenterY(), 0));
	renderer.push(transform);
	renderer.submit(*this);
	renderer.pop();
}
