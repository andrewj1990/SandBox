#pragma once

#include "..\..\Graphics\Sprite.h"
#include "..\..\Entity\Entity.h"

class Bush : public Entity
{
public:
	Bush(int x, int y);
	~Bush() {}

	void damage(int amount) override { m_Destroy = true; }

	void update(float timeElapsed) override;
	void submit(Renderer& renderer) const override;

private:

};