#pragma once

#include "..\Entity.h"

class LifeBar
{
public:
	LifeBar(float width);

	void setPosition(float x, float y);

	void update(float percent, float timeElapsed);
	void render(Renderer& renderer);

private:
	float m_BarWidth;
	float m_DamageBarWidth;

	Sprite m_Background;
	Sprite m_Bar;
	Sprite m_DamageBar;

};