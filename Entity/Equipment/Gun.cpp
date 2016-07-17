#include "Gun.h"

Gun::Gun(float x, float y)
	: m_Sprite(glm::vec3(x, y, 0), glm::vec2(32, 32), TextureManager::get("Textures/Player/Gun.png"))
{
	m_X = x;
	m_Y = y;
}

void Gun::shoot(float x, float y, float angle)
{
	m_Bullets.push_back(Bullet(x, y, angle));
}

void Gun::move(float x, float y)
{
	m_Sprite.addDirection(x, y);
}

void Gun::update(const std::unique_ptr<QuadTree>& quadTree, float timeElapsed)
{
	float mx = Window::Instance().mouseX();
	float my = Window::Instance().mouseY();

	float dx = mx - m_X;
	float dy = my - m_Y;
	float angle = -std::atan2f(dy, dx) - glm::radians(45.0f);

	m_Sprite.setAngle(angle);

	for (Bullet& bullet : m_Bullets)
	{
		bullet.update(timeElapsed);
	}

	for (auto& bullet : m_Bullets)
	{
		std::vector<Renderable*> enemies;

		const glm::vec3& pos = bullet.getSprite().getPosition();
		const glm::vec2& size = bullet.getSprite().getSize();

		quadTree->retrieve(enemies, pos.x, pos.y, size.x, size.y);

		for (const auto& enemy : enemies)
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
				for (int i = 0; i < 100; i++) m_Entities.push_back(std::unique_ptr<Particle>(new Particle(sx, sy)));
				bullet.setDestroy(true);
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
		if ((it)->shouldDestroy())
		{
			it = m_Bullets.erase(it);
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

void Gun::render(Renderer& renderer)
{
	for (Bullet& bullet : m_Bullets)
	{
		bullet.render(renderer);
	}

	renderer.push(glm::mat4(), true);
	for (auto& particle : m_Entities)
	{
		particle->render(renderer);
	}
	renderer.pop();

	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(m_Sprite.getPosition().x + 5, m_Sprite.getPosition().y + 5, 0));
	transform = glm::rotate(transform, m_Sprite.getAngle(), glm::vec3(0, 0, 1));
	transform = glm::translate(transform, glm::vec3(-m_Sprite.getPosition().x - 5, -m_Sprite.getPosition().y - 5, 0));
	
	renderer.push(transform);
	renderer.render(m_Sprite);
	renderer.pop();
}
