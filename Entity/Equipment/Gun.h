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
#include "..\..\Utils\QTree.h"
#include "..\..\Level\Region.h"

class Gun : public Entity
{
public:
	Gun(float x, float y);

	void shoot(float x, float y, float angle);
	void move(float x, float y);

	//void update(const std::unique_ptr<QuadTree>& quadTree, float timeElapsed);
	void update(Region& region, const std::unique_ptr<QTree<Renderable>>& quadTree, float timeElapsed);
	void submit(Renderer& renderer) override;
	void render(Renderer& renderer) override;

	void renderLight(Renderer& renderer) override;

private:
	std::vector<std::unique_ptr<Entity>> m_Entities;
	std::vector<std::unique_ptr<Entity>> m_Bullets;
	std::vector<std::unique_ptr<DamageCounter>> m_DamageText;

};