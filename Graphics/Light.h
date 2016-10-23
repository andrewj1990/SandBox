#pragma once

#include "Renderer.h"
#include "renderable.h"
#include "..\Utils\quadTree.h"

class Light
{
public:
	Light();
	~Light() {}

	void update(float timeElapsed);
	void render(Renderer& renderer);

	int getX() { return m_Point.getPosition().x; }
	int getY() { return m_Point.getPosition().y; }

	const BoundingBox& getLightRegion() { return m_LightRegion; }

private:
	//Renderable m_Point;
	Renderable m_Point;
	Renderable m_LightBox;
	BoundingBox m_LightRegion;

};