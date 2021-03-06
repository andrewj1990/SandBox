#include "BasicMob.h"

BasicMob::BasicMob(float x, float y, std::unique_ptr<Player>& player)
	: Mob(glm::vec3(x, y, 0), glm::vec2(32, 32), TextureManager::get("Textures/Mobs/mob4.png"), player),
	m_Spear(glm::vec3(x, y, 0), glm::vec2(32, 10), TextureManager::get("Textures/Mobs/Spear.png"))
{
	m_MaxHP = 10;
	m_HP = m_MaxHP;

	float sizeFactorX = getWidth() / 32.0f;
	float sizeFactorY = getHeight() / 32.0f;
	m_CollisionBox = BoundingBox(x + (sizeFactorX * 10), y, (sizeFactorX * 10), sizeFactorY + 32);
	//m_Occluder = std::make_shared<BoundingBox>(x + (sizeFactorX * 10), y, (sizeFactorX * 10), sizeFactorY + 32);
	m_Occluder = BoundingBox(x + (sizeFactorX * 10), y, 0, 0);

	m_Actions.push_back(std::make_unique<DamageAction>());
	m_Actions.push_back(std::make_unique<MoveAction>());
	//m_Actions.push_back(std::make_unique<MeleeAction>());
}

BasicMob::~BasicMob()
{
}

void BasicMob::attack(float x, float y)
{
	int fireRingSize = 100;
	m_FireRings.push_back(std::make_unique<Sprite>(glm::vec3(x - fireRingSize / 2, y - fireRingSize / 2, 0), glm::vec2(fireRingSize), TextureManager::get("Textures/aoe.png")));
	auto& fireRing = m_FireRings.back();
	fireRing->setColor(glm::vec4(0, 0, 0, 0.3f));

	m_SpearAttack = 10;
	m_Attacking = true;

}

void BasicMob::damage(int amount)
{
	__super::damage(amount);

	// set to damage action
	m_ActionIndex = 0;
	m_Actions[m_ActionIndex]->init();
}

void BasicMob::update(float timeElapsed)
{
	//m_Angle = Utils::calcAngleRad(getCenterX(), getCenterY(), m_Player->getCenterX(), m_Player->getCenterY());
	if (!m_Attacking)
	{
		m_SpearAttack = 0;
	}
	m_Attacking = false;

	__super::update(timeElapsed);

	if (m_HP <= 0)
	{
		m_Destroy = true;
	}

	playAction(timeElapsed);

	float sizeFactorX = getWidth() / 32.0f;
	float sizeFactorY = getHeight() / 32.0f;
	m_CollisionBox.x = m_Position.x + (sizeFactorX * 10);
	m_CollisionBox.y = m_Position.y;

	m_LifeBar.setPosition(getX(), getY() + sizeFactorY * 32);

	for (auto& it = m_FireRings.begin(); it != m_FireRings.end(); )
	{
		if ((*it)->getSize().x > 300 || (*it)->getColour().x >= 1.0f)
		{
			it = m_FireRings.erase(it);
		}
		else
		{
			(*it)->setSize((*it)->getSize() + glm::vec2(1));
			(*it)->setColor((*it)->getColour() + glm::vec4(0.005f, 0, 0, 0));

			(*it)->addDirection(-0.5f, -0.5f);

			it++;
		}
	}

	m_Spear.setPosition(getX(), getY());

}

void BasicMob::render(Renderer& renderer)
{
	renderer.m_AlphaTest = false;
	renderer.begin();
	for (auto& fireRing : m_FireRings)
	{
		renderer.submit(*fireRing);
	}
	
	renderer.end();
	renderer.flush();
	renderer.m_AlphaTest = true;

	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(getPosition().x + getSize().x / 2.0f, getPosition().y + getSize().y / 2.0f, 0));
	transform = glm::rotate(transform, getAngle(), glm::vec3(0, 0, 1));
	glm::mat4 weapon_transform = transform;
	transform = glm::translate(transform, glm::vec3(-getPosition().x - getSize().x / 2.0f, -getPosition().y - getSize().y / 2.0f, 0));

	weapon_transform = glm::translate(weapon_transform, glm::vec3(-getPosition().x - getSize().x / 2.0f + 20 + m_SpearAttack, -getPosition().y - getSize().y / 2.0f - 5, 0));

	renderer.push(transform);
	renderer.render(*this);
	renderer.pop();

	renderer.push(weapon_transform);
	renderer.render(m_Spear);
	renderer.pop();

	m_LifeBar.render(renderer);
}
