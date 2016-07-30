#pragma once

class Entity;

class Action
{
public:
	Action() { m_Complete = false; }

	virtual void play(Entity& entity) = 0;

	bool isComplete() const { return m_Complete; }

protected:
	bool m_Complete;
};