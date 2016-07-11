#pragma once

#include "..\..\Graphics\Renderer.h"
#include "..\..\Graphics\Sprite.h"
#include "..\..\Utils\quadTree.h"
#include "..\Particle.h"
#include "..\DamageCounter.h"

class Sword
{
public:
	Sword(float x, float y);

	void move(float x, float y);

	void setAttackParams(float angle);
	void setAnimating(bool animating) { m_Animating = animating; }

	void update(const std::unique_ptr<QuadTree>& quadTree, float timeElapsed);
	void render(Renderer& renderer);

private:
	Sprite m_Sprite;

	std::vector<Renderable> m_HitBoxes;
	std::vector<std::unique_ptr<Entity>> m_Entities;

	bool m_Animating;
	float m_SwipeAngle;
	float m_AttackDuration;
	float m_DeltaAngle;
	float m_CumulativeTime;
	float m_StartAngle;
};