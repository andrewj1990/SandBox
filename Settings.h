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
	const int TILE_SIZE = 16;				// size of tiles in px
	const int SUB_REGION_TILE_COUNT = 16;	// number of tiles per sub region
	const float PROJECTION_WIDTH = 1280.0f;	// width and height of projection matrix
	const float PROJECTION_HEIGHT = 720.0f;

	bool debugShowQuadTree = false;
	bool debugShowCollisionBoxes = false;
	bool ADS = true;

	bool noClip = false;

	
private:
	Settings();

	void valueToggle(unsigned int keyCode, bool& toggleVar);
};

