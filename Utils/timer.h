#pragma once
#include <iostream>
#include <ctime>

class Timer
{
public:
	Timer()
	{
		m_Start = clock();
	}

	void reset()
	{
		m_Start = clock();
	}

	float elapsed()
	{
		return (float)(clock() - m_Start) / (float) CLOCKS_PER_SEC;
	}

private:
	double m_Frequency;
	clock_t m_Start;
};