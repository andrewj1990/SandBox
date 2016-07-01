#pragma once

#include <vector>
#include "renderable.h"
#include <algorithm>
#include <iostream>
#include "Layers\layer.h"

class Light
{
public:
	Light(Layer& shadowLayer, const std::vector<Renderable*>& blocks, int x = 0, int y = 0, glm::vec4 color = glm::vec4(0, 0, 0, 1));

	void update(const int& x, const int& y);
	void setRenderables(const std::vector<Renderable*>& blocks);

private:
	void calculateShadow();
	std::vector<glm::vec3> midPoints(Renderable* block);

private:
	int m_X;
	int m_Y;

	Layer& m_Layer;
	const std::vector<Renderable*>& m_Blocks;
	std::vector<std::unique_ptr<Renderable>> m_Shadows;

	glm::vec4 m_Color;
};