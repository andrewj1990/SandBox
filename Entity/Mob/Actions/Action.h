#pragma once

class Entity;

class Action
{
public:
	Action() { m_Complete = false; }
	virtual ~Action() {}

	virtual void init() { m_Complete = false; }
	virtual void play(Entity& mob, Entity& player, float timeElapsed) = 0;

	bool isComplete() const { return m_Complete; }
	void setComplete(bool complete) { m_Complete = complete; }

protected:
	bool m_Complete;

	float m_Duration;
	float m_CumulativeTime;
};