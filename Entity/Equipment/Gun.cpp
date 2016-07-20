#include "Gun.h"

Gun::Gun(float x, float y)
	: Entity(glm::vec3(x, y, 0), glm::vec2(32, 32), TextureManager::get("Textures/Player/Gun.png"))
{
}

void Gun::shoot(float x, float y, float angle)
{
	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(m_Position.x + 5, m_Position.y + 5, 0));
	transform = glm::rotate(transform, getAngle(), glm::vec3(0, 0, 1));
	transform = glm::translate(transform, glm::vec3(-m_Position.x - 5, -m_Position.y - 5, 0));
	const glm::vec3& pos = glm::vec3(m_Position.x + m_Size.x * 0.75f, m_Position.y + m_Size.y * 0.75f, 0);
	glm::vec4 gunPos = transform * glm::vec4(pos, 1.0f);

	for (int i = 0; i < 15; i++) m_Entities.push_back(std::unique_ptr<GunParticle>(new GunParticle(gunPos.x, gunPos.y, angle)));
	m_Bullets.push_back(std::unique_ptr<Bullet>(new Bullet(gunPos.x, gunPos.y, angle)));
}

void Gun::move(float x, float y)
{
	addDirection(x, y);
}

void Gun::update(const std::unique_ptr<QuadTree>& quadTree, float timeElapsed)
{
	float mx = Window::Instance().mouseX();
	float my = Window::Instance().mouseY();

	float dx = mx - m_X;
	float dy = my - m_Y;
	float angle = -std::atan2f(dy, dx) - glm::radians(45.0f);

	setAngle(angle);

	for (auto& bullet : m_Bullets)
	{
		bullet->update(timeElapsed);
	}

	for (auto& bullet : m_Bullets)
	{
		std::vector<Renderable*> enemies;

		const glm::vec3& pos = bullet->getPosition();
		const glm::vec2& size = bullet->getSize();

		quadTree->retrieve(enemies, pos.x, pos.y, size.x, size.y);

		for (auto& enemy : enemies)
		{
			float ex = enemy->getPosition().x;
			float ey = enemy->getPosition().y;
			float ew = enemy->getSize().x;
			float eh = enemy->getSize().y;

			float sx = pos.x;
			float sy = pos.y;
			float sw = size.x;
			float sh = size.y;

			if (sx > ex && sx < ex + ew && sy > ey && sy < ey + eh)
			{
				for (int i = 0; i < 100; i++)
				{
					m_Entities.push_back(std::unique_ptr<Particle>(new Particle(sx, sy, bullet->getAngle())));
				}
				enemy->damage(10);
				//m_DamageText.push_back(std::unique_ptr<DamageCounter>(new DamageCounter("1", sx, sy)));

				bullet->setDestroy(true);
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

	for (auto it = m_Bullets.begin(); it != m_Bullets.end(); )
	{
		if ((*it)->shouldDestroy())
		{
			it = m_Bullets.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (auto it = m_DamageText.begin(); it != m_DamageText.end(); )
	{
		if ((*it)->shouldDestroy())
		{
			it = m_DamageText.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (auto& text : m_DamageText)
	{
		text->update(timeElapsed);
	}

	for (auto& particle : m_Entities)
	{
		particle->update(timeElapsed);
	}
}

void Gun::render(Renderer& renderer)
{
	renderer.push(glm::mat4(), true);
	renderer.render(m_Bullets);
	renderer.render(m_Entities);

	for (auto& text : m_DamageText)
	{
		text->render(renderer);
	}

	renderer.pop();

	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(getPosition().x + 5, getPosition().y + 5, 0));
	transform = glm::rotate(transform, getAngle(), glm::vec3(0, 0, 1));
	transform = glm::translate(transform, glm::vec3(-getPosition().x - 5, -getPosition().y - 5, 0));
	
	renderer.push(transform);
	renderer.render(*this);
	renderer.pop();
}
