#include "ProjectileMob.h"

ProjectileMob::ProjectileMob(float x, float y, std::unique_ptr<Player>& player)
	: Mob(glm::vec3(x, y, 0), glm::vec2(128, 128), TextureManager::get("Textures/Mobs/ro_sprite_evil_fanatic_med.png"), player)
{
	m_MaxHP = 10;
	m_HP = m_MaxHP;

	float sizeFactorX = getWidth() / 32.0f;
	float sizeFactorY = getHeight() / 32.0f;
	m_CollisionBox = std::make_shared<BoundingBox>(x + (sizeFactorX * 10), y, (sizeFactorX * 10), sizeFactorY * 20);
	//m_Occluder = std::make_shared<BoundingBox>(x + (sizeFactorX * 5), y, (sizeFactorX * 10), sizeFactorY * 1);
	m_Occluder = std::make_shared<BoundingBox>(0, 0, 0, 0);

	m_Actions.push_back(std::make_unique<DamageAction>());
	m_Actions.push_back(std::make_unique<MoveAction>());
	//m_Actions.push_back(std::make_unique<AttackAction>());
	m_Actions.push_back(std::make_unique<MeleeAction>());

	m_Anim = 0;
	m_Row = 0;
	setUV(0, 0, 128, 128);
}

ProjectileMob::~ProjectileMob()
{
}

void ProjectileMob::attack(float x, float y)
{
	int fireRingSize = 100;
	float px = m_Player->getCenterX();
	float py = m_Player->getCenterY();
	
	//m_Projectiles.push_back(std::make_unique<FireProjectile>(x, y, Utils::calcAngleRad(x, y, px, py)));
	//auto& fireRing = m_Projectiles.back();
	//fireRing->setColor(glm::vec4(0, 0, 0, 0.3f));

}

void ProjectileMob::damage(int amount)
{
	__super::damage(amount);

	// set to damage action
	m_ActionIndex = 0;
	m_Actions[m_ActionIndex]->init();

}

void ProjectileMob::update(float timeElapsed)
{
	__super::update(timeElapsed);

	if (m_HP <= 0)
	{
		m_Destroy = true;
	}

	playAction(timeElapsed);

	float sizeFactorX = getWidth() / 32.0f;
	float sizeFactorY = getHeight() / 32.0f;
	m_CollisionBox->x = m_Position.x + (sizeFactorX * 10);
	m_CollisionBox->y = m_Position.y;
	//m_Occluder->x = m_Position.x + (sizeFactorX * 5);
	//m_Occluder->y = m_Position.y;

	m_LifeBar.setPosition(getX(), getY() + sizeFactorY * 22);

	for (auto& it = m_Projectiles.begin(); it != m_Projectiles.end(); )
	{
		if ((*it)->shouldDestroy())
		{
			it = m_Projectiles.erase(it);
		}
		else
		{
			(*it)->update(timeElapsed);

			++it;
		}
	}

	m_ReflectX = (m_DirectionX == Direction::RIGHT);
	if (m_DirectionY == Direction::UP)
	{
		m_Row = 3;
	}
	else
	{
		m_Row = 2;
	}

	if (m_State == State::ATTACKING)
	{
		if (m_DirectionY == Direction::UP)
		{
			m_Row = 5;
		}
		else
		{
			m_Row = 4;
		}
	}

	setUV((int)m_Anim % 6, m_Row, 128, 128);

	m_Anim += timeElapsed * 10;

}

void ProjectileMob::render(Renderer& renderer)
{
	renderer.m_AlphaTest = false;
	renderer.begin();
	for (auto& projectile : m_Projectiles)
	{
		projectile->submit(renderer);
	}

	renderer.end();
	renderer.flush();
	renderer.m_AlphaTest = true;

	renderer.render(*this);

	m_LifeBar.render(renderer);

}
