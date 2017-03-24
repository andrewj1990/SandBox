#pragma once

#include "..\..\Utils\TypeID.h"

class Component
{
public:
	Component();
	virtual ~Component();

	int getID() { return m_ID; }

protected:
	int m_ID;

};