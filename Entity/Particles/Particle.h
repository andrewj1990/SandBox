#pragma once

#include "..\..\Utils\Utils.h"
#include "..\Entity.h"

class Particle : public Entity
{
public:
	Particle(float x, float y, float size = 5, float angleDeg = 0, bool applyTransform = false);
	Particle(float x, float y, float size, const glm::vec4& colour, float angleDeg, bool applyTransform = false, float fadeSpeed = 0.2f);
	Particle(float x, float y, float size, Texture* texture, bool applyTransform = false);
	Particle(float x, float y, float size, float angleDeg, Texture* texture, bool applyTransform = false);
	virtual ~Particle() {}
	
	virtual void update(float timeElapsed) override;
	virtual void submit(Renderer& renderer) override;

protected:
	float m_Duration;
	float m_RotationSpeed;
	float m_Dz;
	float m_FadeSpeed;

	bool m_ApplyTransform;
	glm::mat4 m_Transform;
};