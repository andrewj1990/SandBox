#pragma once

#include "..\Entity\Entity.h"

class Tree : public Entity
{
public:
	Tree(int x, int y);

	void update(float timeElapsed) override;
	void render(Renderer& renderer) override;

private:
};