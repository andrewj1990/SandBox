#pragma once

#include "Renderer.h"
#include "renderable.h"
#include "..\Utils\quadTree.h"
#include "Sprite.h"
#include "..\Utils\QTree.h"
#include "..\Utils\Ray.h"
#include "..\Entity\ObjectManager.h"

class SpotLight
{
	glm::vec2 position_;
	float direction_rad_;
	float cone_angle_rad_;

	BoundingBox light_region_;

	std::vector<Renderable> quads_;

public:
	SpotLight();

	void update(float x, float y, float timeElapsed);
	void render(Renderer& renderer);

};