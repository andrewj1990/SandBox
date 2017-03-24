#pragma once

#include <vector>
#include <memory>
#include <iostream>

#include "Components\Component.h"
#include "Components\PositionComponent.h"
#include "..\Utils\TypeID.h"

class TEntity
{
public:
	TEntity();
	~TEntity();

	template<class T>
	void attach(std::shared_ptr<Component> component);

	template<class T>
	void detach();

	template<class T>
	std::shared_ptr<T> get();

	unsigned long getKey() { return m_Key; }
	void setDestroy(bool flag) { m_Destroy = flag; }
	bool shouldDestroy() { return m_Destroy; }

private:
	unsigned long m_Key;
	bool m_Destroy;

	std::vector<std::shared_ptr<Component>> m_Components;
};


template<class T>
inline void TEntity::attach(std::shared_ptr<Component> component)
{
	m_Key |= 1 << TypeID::value<T>();
	m_Components.push_back(component);
}

template<class T>
inline void TEntity::detach()
{
	for (auto& i = m_Components.begin(); i != m_Components.end(); )
	{
		if ((*i)->getID() == TypeID::value<T>())
		{
			i = m_Components.erase(i);
			break;
		}
		i++;
	}
}

template<class T>
inline std::shared_ptr<T> TEntity::get()
{
	for (auto& i = m_Components.begin(); i != m_Components.end(); )
	{
		if ((*i)->getID() == TypeID::value<T>())
		{
			return std::static_pointer_cast<T>(*i);
		}
		i++;
	}

	return nullptr;
}
