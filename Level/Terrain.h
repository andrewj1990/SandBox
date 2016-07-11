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
	glm::vec3 getColor(float height);

	void update(float x, float y, float timeElapsed);
	void render(Renderer& renderer);

private:
	SimplexNoise m_Noise;

	std::vector<Renderable> m_Ground;

};