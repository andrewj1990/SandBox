#pragma once

#include "..\..\Graphics\Sprite.h"
#include "..\Entity.h"
#include "..\..\Graphics\Renderer.h"
#include "..\..\Graphics\Window.h"
#include "..\Projectile\Bullet.h"
#include "..\..\Utils\quadTree.h"
#include "..\Particle.h"

class Gun
{
public:
	Gun(float x, float y);

	void shoot(float x, float y, float angle);
	void move(float x, float y);

	void update(const std::unique_ptr<QuadTree>& quadTree, float timeElapsed);
	void render(Renderer& renderer);

private:
	float m_X;
	float m_Y;
	Sprite m_Sprite;

	std::vector<std::unique_ptr<Entity>> m_Entities;
	std::vector<Bullet> m_Bullets;

};