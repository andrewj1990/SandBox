#pragma once

#include "..\TEntity.h"

class System
{
public:
	System();
	~System();

	unsigned long getLock() { return m_Lock; }
	bool unlock(int key);
	virtual void update(std::shared_ptr<TEntity>& entity, float timeElapsed) = 0;

protected:
	unsigned long m_Lock;

};