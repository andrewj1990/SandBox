#pragma once

#include <cmath>
#include <iostream>

class PerlinNoise
{
public:
	PerlinNoise();

	double calcPerlinNoise(float x, float y);

private:
	double noise(int x, int y);
	double interpolate(float a, float b, float x);
	double smoothNoise(int x, int y);
	double interpolateNoise(float x, float y);

private:
	int m_Frequency;
	float m_Persistence;
	int m_NumOctaves;
};