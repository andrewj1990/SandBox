#include "MeleeMob.h"

MeleeMob::MeleeMob(float x, float y, std::unique_ptr<Player>& player)
	: Mob(glm::vec3(x, y, 0), glm::vec2(32, 32), TextureManager::get("Textures/Mobs/mob4.png"), player),
	m_Spear(glm::vec3(x, y, 0), glm::vec2(32, 10), TextureManager::get("Textures/Mobs/Spear.png")), m_SpearBB(0, 0, 10, 10)
	, m_AttackTime(), m_KnockbackTime()
{
	m_MaxHP = 1;
	m_HP = m_MaxHP;

	m_AttackRange = 50;

	float sizeFactorX = getWidth() / 32.0f;
	float sizeFactorY = getHeight() / 32.0f;
	m_CollisionBox = std::make_shared<BoundingBox>(x + (sizeFactorX * 5), y, (sizeFactorX * 27), sizeFactorY * 32);
	//m_Occluder = std::make_shared<BoundingBox>(x + (sizeFactorX * 10), y, (sizeFactorX * 10), sizeFactorY + 32);
	m_Occluder = std::make_shared<BoundingBox>(x + (sizeFactorX * 10), y, 0, 0);

	m_Movespeed = 100.0f;
	m_SpearAttack = 0;

	m_Attacking = false;
	m_AttackDuration = 0.2f;
	m_AttackSpeed = 100.0f;
	
	m_KnockbackDuration = 0.1f;
}

MeleeMob::~MeleeMob()
{
}

void MeleeMob::attack(float x, float y)
{
	m_SpearAttack = 10;
	m_Attacking = true;

}

void MeleeMob::damage(int amount)
{
	__super::damage(amount);

	//Settings::Instance().timeModifier = 0.5f;

	m_State = State::HIT;
	m_KnockbackTime.reset();

	float randOffset = Utils::random(-30.0f, 30.0f);
	for (int i = 0; i < 50; i++)
	{
		ParticleManager::instance().add(Particle(getCenterX(), getCenterY(), Utils::random(1, 10), glm::vec4(Utils::random(0.8f, 1.0f), 0, 0, Utils::random(0.4f, 1.0f)), glm::degrees(getAngle()) + 180.0f + randOffset));
		ParticleManager::instance().add(Particle(getCenterX(), getCenterY(), Utils::random(1, 10), glm::vec4(Utils::random(0.8f, 1.0f), 0, 0, Utils::random(0.4f, 1.0f)), glm::degrees(getAngle()) + 180.0f + Utils::random(-90.0f, 90.0f)));
	}

	randOffset = Utils::random(-90.0f, 90.0f);
	for (int i = 0; i < 50; i++)
	{
		ParticleManager::instance().add(Particle(getCenterX(), getCenterY(), Utils::random(1, 10), glm::vec4(Utils::random(0.8f, 1.0f), 0, 0, Utils::random(0.4f, 1.0f)), glm::degrees(getAngle()) + 180.0f + randOffset));
	}

	//ParticleManager::instance().add(DamageCounter(std::to_string(amount), getCenterX(), getCenterY()));

}

void MeleeMob::update(float timeElapsed)
{
	__super::update(timeElapsed);

	if (m_HP <= 0)
	{
		m_Destroy = true;
		//ParticleManager::instance().add(Particle(getCenterX(), getCenterY(), getSize().x, glm::degrees(m_Angle), TextureManager::get("Textures/Mobs/mob4_dead.png")));
		ParticleManager::instance().add(Ragdoll(getX(), getY(), getSize().x, glm::degrees(m_Angle), TextureManager::get("Textures/Mobs/mob4_dead.png")));
	}

	m_State = determineState();

	float sizeFactorX = getWidth() / 32.0f;
	float sizeFactorY = getHeight() / 32.0f;
	m_CollisionBox->x = m_Position.x + (sizeFactorX * 5);
	m_CollisionBox->y = m_Position.y;

	m_LifeBar.setPosition(getX(), getY() + sizeFactorY * 32);
	m_Spear.setPosition(getX(), getY());

	glm::mat4 transform;
	m_Angle = Utils::calcAngleRad(getCenterX(), getCenterY(), m_Player->getCenterX(), m_Player->getCenterY());
	transform = glm::translate(transform, glm::vec3(getPosition().x + getSize().x / 2.0f, getPosition().y + getSize().y / 2.0f, 0));
	transform = glm::rotate(transform, m_Angle, glm::vec3(0, 0, 1));
	transform = glm::translate(transform, glm::vec3(-getPosition().x - getSize().x / 2.0f, -getPosition().y - getSize().y / 2.0f, 0));

	const glm::vec3& pos = glm::vec3(m_Spear.getX() + m_Spear.getWidth(), m_Spear.getCenterY(), 0);
	glm::vec4 spearPos = transform * glm::vec4(pos, 1.0f);
	
	m_Angle = Utils::calcAngleRad(spearPos.x, spearPos.y, m_Player->getCenterX(), m_Player->getCenterY());

	glm::mat4 weapon_transform;
	weapon_transform = glm::translate(weapon_transform, glm::vec3(getPosition().x + getSize().x / 2.0f, getPosition().y + getSize().y / 2.0f, 0));
	weapon_transform = glm::rotate(weapon_transform, m_Angle, glm::vec3(0, 0, 1));
	weapon_transform = glm::translate(weapon_transform, glm::vec3(-getPosition().x - getSize().x / 2.0f + 20 + m_SpearAttack, -getPosition().y - getSize().y / 2.0f - 5, 0));

	spearPos = weapon_transform * glm::vec4(pos, 1.0f);

	m_SpearBB.x = spearPos.x - m_SpearBB.width / 2.0f;
	m_SpearBB.y = spearPos.y - m_SpearBB.height / 2.0f;

	switch (m_State)
	{
	case State::ATTACKING:
		attack(timeElapsed);
		break;
	case State::HIT:
		m_SpearAttack = 0;
		knockback(timeElapsed);
		break;
	case State::WALKING:
		m_SpearAttack = 0;
		move(timeElapsed);
		break;
	default:
		break;
	}


}

void MeleeMob::render(Renderer& renderer)
{
	glm::mat4 transform;
	float mobAngle = Utils::calcAngleRad(getCenterX(), getCenterY(), m_Player->getCenterX(), m_Player->getCenterY());
	transform = glm::translate(transform, glm::vec3(getPosition().x + getSize().x / 2.0f, getPosition().y + getSize().y / 2.0f, 0));
	transform = glm::rotate(transform, mobAngle, glm::vec3(0, 0, 1));
	transform = glm::translate(transform, glm::vec3(-getPosition().x - getSize().x / 2.0f, -getPosition().y - getSize().y / 2.0f, 0));

	renderer.push(transform);
	renderer.render(*this);
	renderer.pop();

	glm::mat4 weapon_transform;
	weapon_transform = glm::translate(weapon_transform, glm::vec3(getPosition().x + getSize().x / 2.0f, getPosition().y + getSize().y / 2.0f, 0));
	weapon_transform = glm::rotate(weapon_transform, m_Angle, glm::vec3(0, 0, 1));
	weapon_transform = glm::translate(weapon_transform, glm::vec3(-getPosition().x - getSize().x / 2.0f + 20 + m_SpearAttack, -getPosition().y - getSize().y / 2.0f - 5, 0));

	renderer.push(weapon_transform);
	renderer.render(m_Spear);
	renderer.pop();

	m_LifeBar.render(renderer);
}

void MeleeMob::attack(float timeElapsed)
{
	//m_Angle = Utils::calcAngleRad(getCenterX(), getCenterY(), m_Player->getCenterX(), m_Player->getCenterY());

	m_Attacking = true;
	if (m_AttackTime.elapsed() < m_AttackDuration)
	{
		if (m_SpearBB.intersects(*m_Player->getCollisionBox()))
		{
			m_Player->damage(1);
		}
		m_SpearAttack += m_AttackSpeed * timeElapsed;
	}
	else if (m_AttackTime.elapsed() < m_AttackDuration * 4)
	{
		m_SpearAttack = std::max(0.0f, m_SpearAttack - m_AttackSpeed * 3 * timeElapsed);
	} 
	else 
	{
		m_Attacking = false;
		m_AttackTime.reset();
	}
}

void MeleeMob::move(float timeElapsed)
{
	//m_Angle = Utils::calcAngleRad(getCenterX(), getCenterY(), m_Player->getCenterX(), m_Player->getCenterY());
	
	addDirection(m_Movespeed * std::cosf(m_Angle) * timeElapsed, m_Movespeed * std::sinf(m_Angle) * timeElapsed);
}

void MeleeMob::knockback(float timeElapsed)
{
	//m_Angle = Utils::calcAngleRad(getCenterX(), getCenterY(), m_Player->getCenterX(), m_Player->getCenterY());
	float knockback_angle = m_Angle + glm::radians(180.0f);

	if (m_KnockbackTime.elapsed() < m_KnockbackDuration)
	{
		setColor(glm::vec4(1, 0, 0, 1));
		addDirection(300 * std::cosf(knockback_angle) * timeElapsed, 300 * std::sinf(knockback_angle) * timeElapsed);
	}
	else
	{
		setColor(glm::vec4(1));
		m_State = State::WALKING;
	}
}

State MeleeMob::determineState()
{
	if (m_State == State::HIT) return State::HIT;

	float dSqrd = calcDistanceSqrd(*m_Player);
	
	if (m_Attacking || (m_AttackRange * m_AttackRange) > dSqrd)
	{
		return State::ATTACKING;
	}
	else
	{
		return State::WALKING;
	}

	return State::IDLE;
}
