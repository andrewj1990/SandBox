#pragma once

#include "..\..\Graphics\Sprite.h"
#include "..\Entity.h"
#include "..\..\Graphics\Renderer.h"
#include "..\..\Graphics\Window.h"
#include "..\Projectile\Bullet.h"
#include "..\..\Utils\quadTree.h"
#include "..\Particles\Particle.h"
#include "..\Particles\GunParticle.h"
#include "..\Particles\CollisionParticle.h"
#include "..\DamageCounter.h"
#include "..\..\Utils\QTree.h"
#include "..\..\Level\Region.h"
#include "..\ObjectManager.h"
#include "..\Particles\ParticleManager.h"

class Gun : public Entity
{
public:
	Gun(float x, float y);

	void shoot(float x, float y, float angle, float movespeed = 0.0f);
	void move(float x, float y);

	void update(Region& region, float timeElapsed);
	void render(Renderer& renderer) override;

	void renderLight(Renderer& renderer) override;

private:
	bool m_FaceRight;

	std::vector<std::unique_ptr<Entity>> m_Entities;
	std::vector<std::unique_ptr<Entity>> m_Bullets;
	std::vector<std::unique_ptr<DamageCounter>> m_DamageText;

	Sprite m_Light;

	bool m_Shooting;
	float m_Recoil;
};