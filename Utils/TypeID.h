#pragma once

class TypeID
{
	static int TypeIDCounter;

public:
	template<typename T>
	static int value()
	{
		static int id = TypeIDCounter++;
		return id;
	}
};