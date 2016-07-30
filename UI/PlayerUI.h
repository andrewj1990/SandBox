#pragma once

#include "..\Graphics\Renderer.h"
#include "..\Entity\player.h"

class PlayerUI
{
public:
	PlayerUI(const Player& player);

	void update(float timeElapsed);
	void render(Renderer& renderer);

private:
	const Player& m_Player;

	Sprite m_PlayerHealth;
	Sprite m_Stamina;

};