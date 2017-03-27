#pragma once

#include "..\..\Graphics\Sprite.h"
#include "..\..\Entity\Entity.h"

class Rock : public Entity
{
public:
	Rock(int x, int y);
	~Rock() {}

	void update(float timeElapsed) override;
	void submit(Renderer& renderer) const override;

private:

};