#pragma once

#include "..\Graphics\label.h"
#include "..\Graphics\renderable.h"
#include "Entity.h"

class DamageCounter : public Entity
{
public:
	DamageCounter(const std::string& text, float x, float y);

	void update(float timeElapsed) override;
	void render(Renderer& renderer) override;

private:
	Label m_Label;

	float m_Counter;
};