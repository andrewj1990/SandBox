#pragma once

#include "..\EntityManager.h"

class System
{
public:
	System(std::shared_ptr<EntityManager>& entityManager);
	~System();

	unsigned long getLock() { return m_Lock; }
	bool unlock(int key);
	virtual void update(float timeElapsed) = 0;

protected:
	unsigned long m_Lock;

	std::shared_ptr<EntityManager> m_EntityManager;

};