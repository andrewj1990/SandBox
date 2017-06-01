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
#include "..\Utils\QTree.h"
#include "..\Level\Region.h"
#include "ObjectManager.h"
#include "Projectile\Missile.h"
#include "..\Graphics\SpotLight.h"

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
	void dodge();

	void addBuff();
	void damage(int amount) override;

	void update(Region& region, float timeElapsed);
	void render(Renderer& renderer) override;

	void renderLight(Renderer& renderer) override;

	int getHP() const { return m_HP; }
	int getMaxHP() const { return m_MaxHP; }
	float getHPPercent() const { return std::max(0.0f, (float)m_HP / (float)m_MaxHP); }

	bool isADS() const { return m_AimDownSight; }
	bool isMoving() const { return m_Moving; }
	float getCameraOffsetX() const { return m_CameraOffsetX; }
	float getCameraOffsetY() const { return m_CameraOffsetY; }

private:
	void init();
	bool playerCollision(float dx, float dy);
	void move(Region& region, float timeElapsed);
	void aimDownSight(float timeElapsed);
	void shoot(float angle, float timeElapsed);
	void moveCamera();

private:
	PlayerState m_State;
	int m_TexSize;

	float m_Anim;
	int m_Row;

	float m_CumulativeTime;
	float m_DodgeAngle;

	Gun m_Gun;
	SpotLight line_of_sight_;

	//BoundingBox m_CollisionBox;

	float m_MoveSpeed;
	float m_MoveSlow;
	float m_AttackSpeed;
	float m_AttackFrame;
	float m_CurrentAttackDuration;

	bool m_Moving;
	bool m_AimDownSight;
	float m_AimDownSightTime;
	bool m_AimDownSightZoom;

	float m_CameraOffsetX;
	float m_CameraOffsetY;

	float m_HP;
	float m_MaxHP;

	Timer m_MissileTimer;
};