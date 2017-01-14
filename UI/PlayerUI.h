#pragma once

#include "..\Graphics\Renderer.h"
#include "..\Entity\player.h"

class PlayerUI
{
public:
	PlayerUI(std::shared_ptr<Player> player);

	void update(float timeElapsed);
	void render(Renderer& renderer);

private:
	std::shared_ptr<Player> m_Player;

	Sprite m_PlayerHealth;
	Sprite m_Stamina;
	Sprite m_Crosshair;

};