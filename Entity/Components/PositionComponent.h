#pragma once

#include "Component.h"

struct PositionComponent : public Component
{
	PositionComponent(int x, int y) 
		: Component(), m_X(x), m_Y(y)
	{
		m_ID = TypeID::value<PositionComponent>();
	}

	~PositionComponent() {}
	
	int m_X;
	int m_Y;
};

struct VelocityComponent : public Component
{
	VelocityComponent(int x, int y)
		: Component(), m_X(x), m_Y(y)
	{
		m_ID = TypeID::value<VelocityComponent>();
	}

	~VelocityComponent() {}

	int m_X;
	int m_Y;
};
