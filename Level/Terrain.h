#pragma once

#include "..\Graphics\Renderer.h"
#include "..\Utils\simplexNoise.h"
#include "..\Graphics\renderable.h"
#include "..\Graphics\Window.h"
#include "..\Utils\quadTree.h"

class Terrain
{
public:
	Terrain();

	void init();
	Renderable getTile(float x, float y, float height);
	glm::vec4 getColor(float height);

	bool isSolid(float x, float y) const;

	void update(float timeElapsed);
	void render(Renderer& renderer);

	const std::unique_ptr<QuadTree>& getQuadTree() const { return m_QuadTree; }

private:
	SimplexNoise m_Noise;

	float m_FireRadius;

	int m_Width;
	int m_Height;

	int m_TileSize;
	float m_NoiseSize;

	std::vector<Renderable> m_Ground;
	std::unique_ptr<QuadTree> m_QuadTree;

};