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
#include "..\Utils\QTree.h"
#include "..\Level\Region.h"

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
	void update(Region& region, const std::unique_ptr<QTree<Sprite>>& quadTree, float timeElapsed);
	void submit(Renderer& renderer) override;
	void render(Renderer& renderer) override;

	void renderLight(Renderer& renderer) override;

	bool isADS() const { return m_AimDownSight; }
	float getCameraOffsetX() const { return m_CameraOffsetX; }
	float getCameraOffsetY() const { return m_CameraOffsetY; }

private:
	void init();
	bool playerCollision(float dx, float dy, const std::unique_ptr<QTree<Sprite>>& quadTree);
	void move(const std::unique_ptr<QTree<Sprite>>& quadTree, float timeElapsed);
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

	bool m_ShieldActive;
	Sprite m_Shield;
	Sword m_Sword;
	Gun m_Gun;

	BoundingBox m_CollisionBox;

	Sprite m_Light;

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
};