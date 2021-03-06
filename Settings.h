#pragma once

#include "Graphics\Window.h"

class Settings
{
public:
	static Settings& Instance()
	{
		static Settings instance;
		return instance;
	}

	void settingsToggle();

public:
	const int TILE_SIZE = 32;				// size of tiles in px
	const int SUB_REGION_TILE_COUNT = 10;	// number of tiles per sub region
	const float PROJECTION_WIDTH = 1280.0f;	// width and height of projection matrix
	const float PROJECTION_HEIGHT = 720.0f;

	const float Z_PLANE = (float)(1 << 23);

	bool debugShowQuadTree = false;
	bool debugShowCollisionBoxes = false;
	bool debugShowLightRange = false;
	bool showLevelEditor = false;
	bool ADS = true;

	int drawCount = 0;
	float timeModifier = 1.0f;

	bool noClip = false;

	
private:
	Settings();

	void valueToggle(unsigned int keyCode, bool& toggleVar);
};

