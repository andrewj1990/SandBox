#include "Gun.h"

Gun::Gun(float x, float y)
	: Entity(glm::vec3(x, y, 0), glm::vec2(32, 6), TextureManager::get("Textures/Player/Gun4.png"))
{
	setUV(0, 0, 16, 3);
	m_FaceRight = true;
}

void Gun::shoot(float x, float y, float angle, float movespeed)
{
	glm::mat4 transform;

	int rtx = 3;
	int rty = 3;
	transform = glm::translate(transform, glm::vec3(getPosition().x + rtx, getPosition().y + rty, 0));
	transform = glm::rotate(transform, m_Angle, glm::vec3(0, 0, 1));
	transform = glm::translate(transform, glm::vec3(-getPosition().x - rtx, -getPosition().y - rty, 0));
	const glm::vec3& pos = glm::vec3(getPosition().x + getSize().x - 12, getPosition().y + getSize().y - rty, 0);
	glm::vec4 gunPos = transform * glm::vec4(pos, 1.0f);

	for (int i = 0; i < 150; i++) m_Entities.push_back(std::unique_ptr<GunParticle>(new GunParticle(gunPos.x, gunPos.y, angle, movespeed)));
	m_Bullets.push_back(std::unique_ptr<Bullet>(new Bullet(gunPos.x, gunPos.y, angle)));
}

void Gun::move(float x, float y)
{
	addDirection(x, y);
}

void Gun::update(Region& region, const std::unique_ptr<QTree<Sprite>>& quadTree, float timeElapsed)
{
	float mx = Window::Instance().getMouseWorldPosX();
	float my = Window::Instance().getMouseWorldPosY();

	m_Angle = Utils::calcAngleRad(getCenterX(), getCenterY(), mx, my);

	if (m_FaceRight && (std::abs(glm::degrees(m_Angle)) >= 90.0f))
	{
		addDirection(5, 0);
		setUV(0, 1, 16, 3);
		m_FaceRight = false;
	}
	else if (!m_FaceRight && (std::abs(glm::degrees(m_Angle)) < 90.0f))
	{
		addDirection(-5, 0);
		setUV(0, 0, 16, 3);
		m_FaceRight = true;
	}

	// bullet collision and updates
	for (auto& bullet : m_Bullets)
	{
		bullet->update(timeElapsed);
		if (bullet->shouldDestroy()) continue;

		std::vector<std::shared_ptr<Sprite>> objects;

		float bx = bullet->getCollisionBox()->x;
		float by = bullet->getCollisionBox()->y;
		float bw = bullet->getCollisionBox()->width;
		float bh = bullet->getCollisionBox()->height;

		float bdx = bullet->m_Dx * timeElapsed;
		float bdy = bullet->m_Dy * timeElapsed;

		// used a larger bounding box range because some objects were not being retrieved on the edge cases.
		quadTree->retrieve(objects, BoundingBox(std::fminf(bx, bx + bdx) - 32, std::fminf(by, by + bdy) - 32, bw + std::abs(bdx) + 64, bh + std::abs(bdy) + 64));
		for (auto& object : objects)
		{
			const auto& collisionBox = object->getCollisionBox();

			float cx = collisionBox->x;
			float cy = collisionBox->y;
			float cw = collisionBox->width;
			float ch = collisionBox->height;

			if (Utils::lineIntersection(glm::vec4(bx, by, bx - bullet->m_Dx * timeElapsed, by - bullet->m_Dy * timeElapsed), glm::vec4(cx, cy, cx + cw, cy + ch)) ||
				Utils::lineIntersection(glm::vec4(bx, by, bx - bullet->m_Dx * timeElapsed, by - bullet->m_Dy * timeElapsed), glm::vec4(cx, cy + ch, cx + cw, cy))
				)
			{
				for (int i = 0; i < 25; i++)
				{
					m_Entities.push_back(std::unique_ptr<Particle>(new Particle(cx, cy, 5.0f, glm::degrees(bullet->getAngle()))));
				}

				//region.removeTiles(collisionBox->x, collisionBox->y, true, true);
				object->setDestroy(true);
				bullet->setDestroy(true);
				break;
			}
		}
	}

	// entity updates
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
		if ((*it)->shouldDestroy() && (*it)->getSize().x <= 0)
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
	renderer.begin();
	renderer.m_AlphaTest = false;

	glEnable(GL_DEPTH_TEST);
	for (auto& bullet : m_Bullets) bullet->submit(renderer);
	for (auto& entity : m_Entities) entity->submit(renderer);

	renderer.end();
	renderer.flush();
	renderer.m_AlphaTest = true;
	glDisable(GL_DEPTH_TEST);

	for (auto& text : m_DamageText) text->render(renderer);

	renderer.pop();

	int rtx = 3;
	int rty = 3;
	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(getPosition().x + rtx, getPosition().y + rty, 0));
	transform = glm::rotate(transform, m_Angle, glm::vec3(0, 0, 1));
	transform = glm::translate(transform, glm::vec3(-getPosition().x - rtx, -getPosition().y - rty, 0));

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
