#pragma once

// class made with the info from: http://ncase.me/sight-and-light/

#include "Renderer.h"
#include "renderable.h"
#include "..\Utils\quadTree.h"
#include "Sprite.h"
#include "..\Utils\QTree.h"
#include "..\Utils\Ray.h"
#include "..\Entity\ObjectManager.h"

class PointLight
{
public:
	PointLight();
	PointLight(const PointLight& other);
	~PointLight() {}

	void update(float x, float y, float timeElapsed);
	void render(Renderer& renderer);

	int getX() { return m_Position.x; }
	int getY() { return m_Position.y; }

	const BoundingBox& getLightRegion() { return m_LightRegion; }

private:
	glm::vec2 m_Position;
	BoundingBox m_LightRegion;

	//std::vector<std::unique_ptr<Renderable>> m_LightQuads;
	std::vector<Renderable> m_Quads;
};