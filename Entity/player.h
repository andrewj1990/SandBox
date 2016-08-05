#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "Entity.h"
#include "..\SkeletalAnimation\Skeleton.h"
#include "..\Graphics\Window.h"
#include "..\Utils\ResourceManager.h"
#include "Equipment\Sword.h"
#include "Equipment\Gun.h"
#include "Projectile\Bullet.h"
#include "..\Utils\quadTree.h"
#include "..\Level\Terrain.h"

enum PlayerState
{
	NORMAL,
	DODGE,
	ATTACK
};

class Player : public Entity
{
public:
	Player(float x, float y);

	void move(float dx, float dy);
	void dodge(const Terrain& terrain);

	void addBuff();

	void update(float timeElapsed) override;
	void update(const Terrain& terrain, const std::unique_ptr<QuadTree>& quadTree, float timeElapsed) override;
	void render(Renderer& renderer) override;

private:
	void init();
	bool playerCollision(const std::unique_ptr<QuadTree>& quadTree);
	void move(const Terrain& terrain, float timeElapsed);
	void shoot(float angle, float timeElapsed);

private:
	PlayerState m_State;

	float m_CumulativeTime;
	float m_DodgeAngle;

	bool m_ShieldActive;
	Sprite m_Shield;
	Sword m_Sword;
	Gun m_Gun;

	float m_MoveSpeed;
	float m_AttackSpeed;
	float m_AttackFrame;

};