#pragma once

#include "..\Entity.h"
#include "..\player.h"
#include "Actions\DamageAction.h"
#include "LifeBar.h"

enum class Direction
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

enum class State
{
	IDLE,
	WALKING,
	ATTACKING,
	HIT
};

class Mob : public Entity
{
public:
	Mob(const glm::vec3& position, const glm::vec2& size, Texture* texture, std::unique_ptr<Player>& player);
	~Mob() {}

	State getState() const { return m_State; }
	void setState(State state) { m_State = state; }

	void setDirectionX(Direction direction) { m_DirectionX = direction; }
	void setDirectionY(Direction direction) { m_DirectionY = direction; }

	virtual void damage(float amount);
	virtual void attack(float x, float y) {}

	virtual void update(float timeElapsed);

	void playAction(float timeElapsed);

protected:
	float m_HP;
	float m_MaxHP;
	int m_ActionIndex;

	std::unique_ptr<Player>& m_Player;
	LifeBar m_LifeBar;

	std::vector<std::unique_ptr<Action>> m_Actions;

	State m_State;
	Direction m_DirectionX;
	Direction m_DirectionY;
};