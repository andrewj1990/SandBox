#pragma once

#include "..\..\Graphics\Sprite.h"
#include "..\Entity.h"
#include "..\..\Graphics\Renderer.h"
#include "..\..\Graphics\Window.h"
#include "..\Projectile\Bullet.h"
#include "..\..\Utils\quadTree.h"
#include "..\Particle.h"
#include "..\GunParticle.h"
#include "..\DamageCounter.h"

class Gun : public Entity
{
public:
	Gun(float x, float y);

	void shoot(float x, float y, float angle);
	void move(float x, float y);

	void update(const std::unique_ptr<QuadTree>& quadTree, float timeElapsed);
	void render(Renderer& renderer);

private:
	std::vector<std::unique_ptr<Renderable>> m_Entities;
	std::vector<std::unique_ptr<Renderable>> m_Bullets;
	std::vector<std::unique_ptr<DamageCounter>> m_DamageText;

};