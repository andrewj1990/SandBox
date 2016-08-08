#pragma once

#include "..\Entity\Entity.h"

class Tree : public Entity
{
public:
	Tree();

	bool shouldDestroy() const override;

	void update(float timeElapsed) override;
	void render(Renderer& renderer) override;

private:
};