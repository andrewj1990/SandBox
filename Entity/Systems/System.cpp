#include "System.h"

System::System(std::shared_ptr<EntityManager>& entityManager)
	: m_EntityManager(entityManager), m_Lock(0)
{
}

System::~System()
{
}

bool System::unlock(int key)
{
	//std::cout << "lock : " << m_Lock << "\n";
	//std::cout << "key  : " << key << "\n";
	if (m_Lock & key == m_Lock) return true;
	return false;
}
