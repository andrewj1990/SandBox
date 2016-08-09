#pragma once

#include "..\Graphics\Renderer.h"
#include "..\Utils\simplexNoise.h"
#include "..\Graphics\renderable.h"
#include "..\Graphics\Window.h"
#include "..\Utils\quadTree.h"
#include "Tiles\Tile.h"

class Terrain
{
public:
	Terrain();

	void init();
	void addPlayerPtr();

	Renderable getTile(float x, float y, float height);
	glm::vec4 getColor(float height);

	bool isSolid(float x, float y) const;

	void update(float timeElapsed);
	void render(Renderer& renderer);

	std::vector<std::shared_ptr<Entity>>& getObjects() { return m_Objects; }

private:
	SimplexNoise m_Noise;

	float m_FireRadius;

	int m_Width;
	int m_Height;

	// buffer offsets to render tiles outside view
	int m_WidthOffset;
	int m_HeightOffset;

	int m_TileSize;
	float m_NoiseSize;

	std::vector<std::unique_ptr<Renderable>> m_Tiles;
	std::vector<std::shared_ptr<Entity>> m_Objects;
};