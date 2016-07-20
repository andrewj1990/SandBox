#include "Utils.h"

float Utils::random()
{
	float r = rand();
	return r;
}

int Utils::random(int lowerBound, int upperBound)
{
	int range = upperBound - lowerBound;
	float r = rand() % range;
	return lowerBound + r;
}

float Utils::random(float lowerBound, float upperBound)
{
	float r = lowerBound + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (upperBound - lowerBound)));
	return r;
}


