#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "Entity.h"
#include "..\SkeletalAnimation\Skeleton.h"
#include "..\Graphics\Window.h"
#include "..\Utils\ResourceManager.h"
#include "Equipment\Sword.h"
#include "..\Utils\quadTree.h"

enum PlayerState
{
	NORMAL,
	DODGE,
	ATTACK
};

class Player : public Entity
{
public:
	Player();
	Player(float x, float y);

	void move(float dx, float dy);
	void dodge();

	void update(const std::unique_ptr<QuadTree>& quadTree, float timeElapsed);
	void update(float timeElapsed) override;
	void render(Renderer& renderer) override;

private:
	void init();

	PlayerState m_State;

	float m_CumulativeTime;
	float m_DodgeAngle;

	bool m_ShieldActive;
	Sprite m_Shield;
	Sword m_Sword;
};