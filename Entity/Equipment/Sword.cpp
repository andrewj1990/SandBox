#include "Sword.h"

Sword::Sword(float x, float y)
	: m_Sprite(glm::vec3(x, y, 0), glm::vec2(32, 32), TextureManager::get("Textures/Player/Sword.png"))
{
	m_CumulativeTime = 0.0f;
	m_StartAngle = glm::radians(-45.0f);
	//m_Sprite.setAngle(m_StartAngle);
	m_AttackDuration = 0.2f;
	m_SwipeAngle = 180.0f;
	m_DeltaAngle = 0.0f;

	for (int i = 2; i < 6; i++)
	{
		m_HitBoxes.push_back(Entity(glm::vec3(x + i * 5, y + i * 5, 0), glm::vec2(5, 5), glm::vec4(1, 1, 1, 0.5)));
	}
}

void Sword::move(float x, float y)
{
	m_Sprite.addDirection(x, y);

	for (Entity& hitBoxes : m_HitBoxes)
	{
		hitBoxes.getSprite().addDirection(x, y);
	}
}

void Sword::setAttackParams(float angle)
{
	if (!m_Animating)
	{
		m_StartAngle = angle - glm::radians(90.0f + 45.0f);
		m_Sprite.setAngle(m_StartAngle);
		m_DeltaAngle = glm::radians(m_SwipeAngle / m_AttackDuration);
		m_Animating = true;
	}
}

void Sword::update(const std::unique_ptr<QuadTree>& quadTree, float timeElapsed)
{
	if (m_Animating)
	{
		m_CumulativeTime += timeElapsed;
		if (m_CumulativeTime <= m_AttackDuration)
		{
			float currentAngle = m_Sprite.getAngle();
			m_Sprite.setAngle(currentAngle + m_DeltaAngle * timeElapsed);
		}
		else
		{
			m_CumulativeTime = 0.0f;
			m_Sprite.setAngle(0.0f);
			m_Animating = false;
		}

		for (const auto& swordHitBox : m_HitBoxes)
		{
			std::vector<Entity*> enemies;

			glm::mat4 transform;
			transform = glm::translate(transform, glm::vec3(m_Sprite.getPosition().x + 5, m_Sprite.getPosition().y + 5, 0));
			transform = glm::rotate(transform, m_Sprite.getAngle(), glm::vec3(0, 0, 1));
			transform = glm::translate(transform, glm::vec3(-m_Sprite.getPosition().x - 5, -m_Sprite.getPosition().y - 5, 0));
			const glm::vec3& pos = swordHitBox.getSprite().getPosition();
			glm::vec4 swordPos = transform * glm::vec4(pos, 1.0f);

			quadTree->retrieve(enemies, swordPos.x, swordPos.y, swordHitBox.getWidth(), swordHitBox.getHeight());

			for (Entity* enemy : enemies)
			{
				float ex = enemy->getSprite().getPosition().x;
				float ey = enemy->getSprite().getPosition().y;
				float ew = enemy->getSprite().getSize().x;
				float eh = enemy->getSprite().getSize().y;

				float sx = swordPos.x;
				float sy = swordPos.y;
				float sw = swordHitBox.getWidth();
				float sh = swordHitBox.getHeight();

				if (Utils::quadCollision(sx, sy, sw, sh, ex, ey, ew, eh))
				{
					m_Entities.push_back(std::unique_ptr<Particle>(new Particle(sx, sy)));
					enemy->damage(10);
				}

				if (sx > ex && sx < ex + ew && sy > ey && sy < ey + eh)
				{
					//m_Entities.push_back(std::unique_ptr<DamageCounter>(new DamageCounter("1", sx, sy)));
				}

			}
		}

	}

	for (auto it = m_Entities.begin(); it != m_Entities.end(); )
	{
		if ((*it)->shouldDestroy())
		{
			it = m_Entities.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (auto& particle : m_Entities)
	{
		particle->update(timeElapsed);
	}
}

void Sword::render(Renderer & renderer)
{
	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(m_Sprite.getPosition().x + 5, m_Sprite.getPosition().y + 5, 0));
	transform = glm::rotate(transform, m_Sprite.getAngle(), glm::vec3(0, 0, 1));
	transform = glm::translate(transform, glm::vec3(-m_Sprite.getPosition().x - 5, -m_Sprite.getPosition().y - 5, 0));
	renderer.push(transform);
	renderer.render(m_Sprite);
	//renderer.render(m_HitBoxes);
	renderer.pop();

	renderer.push(glm::mat4(), true);
	for (auto& particle : m_Entities)
	{
		particle->render(renderer);
	}
	renderer.pop();

}
