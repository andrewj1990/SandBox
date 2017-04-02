#include "CollisionParticle.h"

CollisionParticle::CollisionParticle(float x, float y, float angleDeg)
	: Entity(glm::vec3(x, y, 5000), glm::vec2(3, Utils::random(2, Utils::random(3,6))), glm::vec4(Utils::random(0.9f, 1.0f), Utils::random(0.6f, 1.0f), Utils::random(0.0f, 0.2f), 1.0f))
{
	m_Angle = angleDeg;
	init();
}

CollisionParticle::CollisionParticle(float x, float y, float angleDeg, Texture* texture)
	: Entity(glm::vec3(x, y, 5000), glm::vec2(3, Utils::random(2, Utils::random(3, 6))), texture)
{
	int w = Utils::random(2, texture->getWidth());
	int h = Utils::random(2, texture->getHeight());
	int ux = Utils::random(0, texture->getWidth() / w);
	int uy = Utils::random(0, texture->getHeight() / h);
	setUV(ux, uy, w, h);

	m_Angle = angleDeg;
	init();
}

void CollisionParticle::init()
{
	float angleOffset = Utils::random(-90.0f, 90.0f);

	float vel = Utils::random(50.0f, 150.0f);
	float a = glm::radians(m_Angle + angleOffset + 180.0f);

	m_Dx = std::cosf(a) * vel;
	m_Dy = std::sinf(a) * vel;

	m_Duration = Utils::random(0.1f, 0.5f);
	m_Angle = a;

	m_StartDestruction = false;
}

void CollisionParticle::update(float timeElapsed)
{
	float min = 10;
	float max = 50;
	float deltaSize = Utils::random(min, max) * timeElapsed;
	float deltaY = 0.05;
	if (m_StartDestruction)
	{
		m_Size.x -= deltaSize * 2;
		m_Size.y -= deltaY * 2;
	}
	

	if (m_Size.x < 0) m_Size.x = 0;
	else m_Size.x += deltaSize;
	
	if (m_Size.y < 0) m_Size.y = 0;
	//else m_Size.y += deltaY;


	m_Duration -= timeElapsed;

	addDirection(m_Dx * timeElapsed, m_Dy * timeElapsed);

	if (m_Duration <= 0)
	{
		m_StartDestruction = true;
	}

	if (m_Size.x <= 0 || m_Size.y <= 0) {
		m_Destroy = true;
	}
}

void CollisionParticle::submit(Renderer & renderer)
{
	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(m_Position.x, m_Position.y + m_Size.y / 2.0f, 0));
	transform = glm::rotate(transform, m_Angle + glm::radians(180.0f), glm::vec3(0, 0, 1));
	transform = glm::translate(transform, glm::vec3(-m_Position.x, -m_Position.y - m_Size.y / 2.0f, 0));
	renderer.push(transform);
	renderer.submit(*this);
	renderer.pop();
}
