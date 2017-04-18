#pragma once

#include "..\..\Graphics\Sprite.h"
#include "..\..\Entity\Entity.h"

class Rock : public Entity
{
public:
	Rock(int x, int y);
	~Rock() {}

	void damage(int amount) override { m_Destroy = true; }

	void update(float timeElapsed) override;
	void submit(Renderer& renderer) const override;

private:

};