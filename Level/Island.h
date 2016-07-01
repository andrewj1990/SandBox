#pragma once

#include "..\Utils\simplexNoise.h"

class Island
{
public:
	Island();

	void init();

	void update();
	void render();

private:
	SimplexNoise m_Noise;

};