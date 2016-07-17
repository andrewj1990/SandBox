#pragma once

#include "..\Graphics\Renderer.h"
#include "..\Utils\simplexNoise.h"
#include "..\Graphics\renderable.h"
#include "..\Graphics\Window.h"

class Terrain
{
public:
	Terrain();

	void init();
	Renderable getTile(float x, float y, float height);
	glm::vec4 getColor(float height);

	bool isCollidable(float x, float y) const;

	void update(float timeElapsed);
	void render(Renderer& renderer);

private:
	SimplexNoise m_Noise;

	int m_Width;
	int m_Height;

	std::vector<Renderable> m_Ground;

};