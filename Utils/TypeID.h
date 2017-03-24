#pragma once

class TypeID
{
public:
	template<typename T>
	static int value()
	{
		static int id = TypeIDCounter++;
		return id;
	}

private:
	static int TypeIDCounter;

};
