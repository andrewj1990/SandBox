#pragma once

#include "..\Entity.h"
#include "..\Particles\Particle.h"
#include "..\ObjectManager.h"

class Missile : public Entity
{
public:
	Missile(float x, float y, Entity* target);
	~Missile() {}

	void update(float timeElapsed) override;
	void submit(Renderer& renderer) override;

private:
	void retarget();
	void angleDelta(float timeElapsed);

private:
	Entity* m_Target;

	float m_Velocity;
	float m_AngleDelta;
	float m_TargetTime;

	glm::mat4 m_Transform;

};