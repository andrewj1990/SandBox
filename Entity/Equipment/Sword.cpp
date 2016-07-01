#include "Sword.h"

Sword::Sword(float x, float y)
	: m_Sprite(glm::vec3(x, y, 0), glm::vec2(32, 32), TextureManager::get("Textures/Player/Sword.png"))
{
	m_AnimDuration = 2.0f;
	m_CumulativeTime = 0.0f;
	m_CurrentAngle = 0.0f;
	m_StartAngle = 0.0f;
	m_EndAngle = 180.0f;

	for (int i = 2; i < 6; i++)
	{
		m_HitBoxes.push_back(Sprite(glm::vec3(x + i * 5, y + i * 5, 0), glm::vec2(5, 5), glm::vec4(1, 1, 1, 0.5)));
	}
}

void Sword::move(float x, float y)
{
	m_Sprite.addDirection(x, y);

	for (Renderable& hitBoxes : m_HitBoxes)
	{
		hitBoxes.addDirection(x, y);
	}
}

void Sword::update(const std::unique_ptr<QuadTree>& quadTree, float timeElapsed)
{
	if (m_Animating)
	{
		m_CumulativeTime += timeElapsed;
		float rotation = 0.0f;
		float currentAngle = m_Sprite.getAngle();
		if (glm::degrees(currentAngle) < m_EndAngle)
		{
			float angle = (m_EndAngle - m_StartAngle) / (m_AnimDuration - m_CumulativeTime);
			m_Sprite.setAngle(currentAngle + glm::radians(m_EndAngle * timeElapsed * 5.0f));
		}
		else
		{
			m_CumulativeTime = 0.0f;
			m_Sprite.setAngle(0.0f);
			m_Animating = false;
		}

		for (const auto& swordHitBox : m_HitBoxes)
		{
			std::vector<Renderable*> enemies;

			glm::mat4 transform;
			transform = glm::translate(transform, glm::vec3(m_Sprite.getPosition().x + 5, m_Sprite.getPosition().y + 5, 0));
			transform = glm::rotate(transform, m_Sprite.getAngle(), glm::vec3(0, 0, 1));
			transform = glm::translate(transform, glm::vec3(-m_Sprite.getPosition().x - 5, -m_Sprite.getPosition().y - 5, 0));
			const glm::vec3& pos = swordHitBox.getPosition();
			glm::vec4 swordPos = transform * glm::vec4(pos, 1.0f);

			quadTree->retrieve(enemies, swordPos.x, swordPos.y, swordHitBox.getSize().x, swordHitBox.getSize().y);

			for (const auto& enemy : enemies)
			{
				float ex = enemy->getPosition().x;
				float ey = enemy->getPosition().y;
				float ew = enemy->getSize().x;
				float eh = enemy->getSize().y;

				float sx = swordPos.x;
				float sy = swordPos.y;
				float sw = swordHitBox.getSize().x;
				float sh = swordHitBox.getSize().y;

				if (sx > ex && sx < ex + ew && sy > ey && sy < ey + eh)
				{
					m_Entities.push_back(std::unique_ptr<Particle>(new Particle(sx, sy)));
					//m_Entities.push_back(std::unique_ptr<DamageCounter>(new DamageCounter("1", sx, sy)));
					//std::cout << "player collision\n";
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
	//std::cout << "hitbox : " << m_HitBoxes[0].getPosition().x << ", " << m_HitBoxes[0].getPosition().y << "\n";

	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(m_Sprite.getPosition().x + 5, m_Sprite.getPosition().y + 5, 0));
	transform = glm::rotate(transform, m_Sprite.getAngle(), glm::vec3(0, 0, 1));
	transform = glm::translate(transform, glm::vec3(-m_Sprite.getPosition().x - 5, -m_Sprite.getPosition().y - 5, 0));
	renderer.push(transform);
	renderer.render(m_Sprite);
	renderer.render(m_HitBoxes);
	renderer.pop();

	//renderer.render(m_HitParticles);

	for (auto& particle : m_Entities)
	{
		particle->render(renderer);
	}

}
