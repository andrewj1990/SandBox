#pragma once

#include "..\..\Graphics\Sprite.h"
#include "..\..\Entity\Entity.h"

class Tree : public Entity
{
public:
	Tree(int x, int y);
	~Tree() {}

	void update(float timeElapsed) override;
	void submit(Renderer& renderer) const override;

private:

};