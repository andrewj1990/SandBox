#pragma once

#include "..\Entity.h"

class BasicMob : public Entity
{
public:
	BasicMob(float x, float y);
	~BasicMob();

	void update(float timeElapsed) override;
	void render(Renderer& renderer) override;

private:

};