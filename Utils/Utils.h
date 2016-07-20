#pragma once

#include <cstdlib>

class Utils
{
public:
	static float random();
	static int random(int lowerBound, int upperBound);
	static float random(float lowerBound, float upperBound);
};