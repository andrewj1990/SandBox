#pragma once

#include "..\..\Graphics\Sprite.h"

class Tree : public Sprite
{
public:
	Tree(int x, int y);
	~Tree() {}

	void update(float timeElapsed);
	void submit(Renderer& renderer) const override;

private:

};