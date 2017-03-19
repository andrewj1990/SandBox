#pragma once

#include "..\..\Graphics\Sprite.h"

class Rock : public Sprite
{
public:
	Rock(int x, int y);
	~Rock() {}

	void update(float timeElapsed);
	void submit(Renderer& renderer) const override;

private:

};