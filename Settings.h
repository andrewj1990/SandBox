#pragma once

struct Settings
{
	constexpr static int TILE_SIZE = 16;				// size of tiles in px
	constexpr static int SUB_REGION_TILE_COUNT = 16;	// number of tiles per sub region
	constexpr static float PROJECTION_WIDTH = 1280.0f;	// width and height of projection matrix
	constexpr static float PROJECTION_HEIGHT = 720.0f;
};

