#include "Gun.h"

Gun::Gun(float x, float y)
	: Entity(glm::vec3(x, y, 0), glm::vec2(32, 10), TextureManager::get("Textures/Player/Gun2.png"))
{
	m_Sprite.setUV(0, 0, 10, 3);
}

void Gun::shoot(float x, float y, float angle)
{
	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(m_Sprite.getPosition().x + 5, m_Sprite.getPosition().y + 5, 0));
	transform = glm::rotate(transform, m_Sprite.getAngle(), glm::vec3(0, 0, 1));
	transform = glm::translate(transform, glm::vec3(-m_Sprite.getPosition().x - 5, -m_Sprite.getPosition().y - 5, 0));
	const glm::vec3& pos = glm::vec3(m_Sprite.getPosition().x + m_Sprite.getSize().x + 5, m_Sprite.getPosition().y + m_Sprite.getSize().y - 5, 0);
	glm::vec4 gunPos = transform * glm::vec4(pos, 1.0f);

	float mx = Window::Instance().mouseX() + Window::Instance().getCamera().getPosition().x;
	float my = Window::Instance().mouseY() + Window::Instance().getCamera().getPosition().y;

	float dx = mx - gunPos.x;
	float dy = my - gunPos.y;

	float a = -std::atan2f(dy, dx);

	for (int i = 0; i < 15; i++) m_Entities.push_back(std::unique_ptr<GunParticle>(new GunParticle(gunPos.x, gunPos.y, angle)));
	m_Bullets.push_back(std::unique_ptr<Bullet>(new Bullet(gunPos.x, gunPos.y, angle)));
}

void Gun::move(float x, float y)
{
	m_Sprite.addDirection(x, y);
}

void Gun::update(const std::unique_ptr<QuadTree>& quadTree, float timeElapsed)
{
	float mx = Window::Instance().mouseX();
	float my = Window::Instance().mouseY();

	float dx = mx - Window::Instance().getWidth() / 2 - 16.0f + 2;
	float dy = my - Window::Instance().getHeight() / 2 - 16.0f + 8;
	float angle = -std::atan2f(dy, dx);// -glm::radians(45.0f);

	if (glm::degrees(angle) > 90.0f ||  glm::degrees(angle) < -90.0f)
	{
		m_Sprite.setUV(0, 1, 10, 3);
	}
	else
	{
		m_Sprite.setUV(0, 0, 10, 3);
	}

	m_Sprite.setAngle(angle);

	for (auto& bullet : m_Bullets)
	{
		bullet->update(timeElapsed);
		std::vector<Entity*> enemies;

		const glm::vec3& pos = bullet->getSprite().getPosition();
		const glm::vec2& size = bullet->getSprite().getSize();

		quadTree->retrieve(enemies, pos.x, pos.y, size.x, size.y);

		for (auto& enemy : enemies)
		{
			float ex = enemy->getSprite().getPosition().x;
			float ey = enemy->getSprite().getPosition().y;
			float ew = enemy->getSprite().getSize().x;
			float eh = enemy->getSprite().getSize().y;

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

void Gun::submit(Renderer& renderer)
{
	renderer.push(glm::mat4(), true);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (auto& bullet : m_Bullets) bullet->submit(renderer);
	for (auto& entity : m_Entities) entity->submit(renderer);
	//renderer.end();
	//renderer.flush();
	//renderer.begin();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (auto& text : m_DamageText)
	{
		text->render(renderer);
	}

	renderer.pop();

	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(m_Sprite.getPosition().x + 5, m_Sprite.getPosition().y + 5, 0));
	transform = glm::rotate(transform, m_Sprite.getAngle(), glm::vec3(0, 0, 1));
	transform = glm::translate(transform, glm::vec3(-m_Sprite.getPosition().x - 5, -m_Sprite.getPosition().y - 5, 0));

	renderer.push(transform);
	renderer.submit(m_Sprite);
	renderer.pop();
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
	transform = glm::translate(transform, glm::vec3(m_Sprite.getPosition().x + 5, m_Sprite.getPosition().y + 5, 0));
	transform = glm::rotate(transform, m_Sprite.getAngle(), glm::vec3(0, 0, 1));
	transform = glm::translate(transform, glm::vec3(-m_Sprite.getPosition().x - 5, -m_Sprite.getPosition().y - 5, 0));

	
	renderer.push(transform);
	renderer.render(*this);
	renderer.pop();
}

void Gun::renderLight(Renderer& renderer)
{
	for (auto& bullet : m_Bullets)
	{
		bullet->renderLight(renderer);
	}
}
