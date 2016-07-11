#pragma once

#include "..\..\Graphics\Sprite.h"
#include "..\Entity.h"
#include "..\..\Graphics\Renderer.h"
#include "..\..\Graphics\Window.h"

class Gun
{
public:
	Gun(float x, float y);

	void move(float x, float y);

	void update(float timeElapsed);
	void render(Renderer& renderer);

private:
	float m_X;
	float m_Y;
	Sprite m_Sprite;

	std::vector<std::unique_ptr<Entity>> m_Entities;

};