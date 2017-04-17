#pragma once

#include "..\Entity.h"
#include "..\player.h"

class Mob : public Entity
{
public:
	Mob(const glm::vec3& position, const glm::vec2& size, Texture* texture, std::unique_ptr<Player>& player);
	~Mob() {}

	void playAction(float timeElapsed);

protected:
	int m_Life;
	int m_ActionIndex;

	std::unique_ptr<Player>& m_Player;

	std::vector<std::unique_ptr<Action>> m_Actions;

};