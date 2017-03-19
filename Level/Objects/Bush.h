#pragma once

#include "..\..\Graphics\Sprite.h"

class Bush : public Sprite
{
public:
	Bush(int x, int y);
	~Bush() {}

	void update(float timeElapsed);
	void submit(Renderer& renderer) const override;

private:

};