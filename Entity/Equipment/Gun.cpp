#include "Gun.h"

Gun::Gun(float x, float y)
	: Entity(glm::vec3(x, y, 0), glm::vec2(32, 10), TextureManager::get("Textures/Player/pistol.png"))
{
	//setUV(0, 0, 32, 10);
	m_FaceRight = true;

	m_Recoil = 0;
	m_Shooting = false;

	m_Light = Sprite(glm::vec3(0, 0, 0), glm::vec2(100, 100), TextureManager::get("Textures/Bullet2.png"));
}

void Gun::shoot(float x, float y, float angle, float movespeed)
{
	glm::mat4 transform;

	int rtx = 16;
	int rty = 16;
	transform = glm::translate(transform, glm::vec3(getPosition().x + rtx, getPosition().y + rty, 0));
	transform = glm::rotate(transform, m_Angle, glm::vec3(0, 0, 1));
	transform = glm::translate(transform, glm::vec3(-getPosition().x - 1 + m_Recoil, -getPosition().y - 23, 0));
	const glm::vec3& pos = glm::vec3(getPosition().x + getSize().x + 5, getPosition().y + getSize().y - 5, 0);
	glm::vec4 gunPos = transform * glm::vec4(pos, 1.0f);

	//float newAngle = Utils::calcAngleRad(gunPos.x, gunPos.y, Window::Instance().getMouseWorldPosX(), Window::Instance().getMouseWorldPosY());
	for (int i = 0; i < 100; i++) m_Entities.push_back(std::make_unique<GunParticle>(gunPos.x, gunPos.y, m_Angle, movespeed));
	for (int i = 0; i < 100; i++) m_Entities.push_back(std::make_unique<GunParticle>(gunPos.x, gunPos.y, m_Angle, movespeed, 20.0f, 400.0f, 1000.0f));
	m_Bullets.push_back(std::make_unique<Bullet>(gunPos.x, gunPos.y, m_Angle + angle));

	m_Light.setSize(glm::vec2(Utils::random(100, 350)));
	m_Light.setPosition(gunPos.x - m_Light.getSize().x / 2, gunPos.y - m_Light.getSize().y / 2);
	m_Light.setLightPosition(gunPos.x, gunPos.y, m_Light.getSize().x / 2);

	const glm::vec3& shellOffset = glm::vec3(getPosition().x + getSize().x - 20, getPosition().y + getSize().y - 5, 0);
	glm::vec4 shellPos = transform * glm::vec4(shellOffset, 1.0f);
	ParticleManager::instance().add(Particle(shellPos.x, shellPos.y, 10, glm::degrees(m_Angle) + 180.0f + Utils::random(-30.0f, 30.0f), TextureManager::get("Textures/Player/bullet_shell.png"), true));

	m_Shooting = true;
	m_Recoil = -5;
}

void Gun::move(float x, float y)
{
	addDirection(x, y);
}

void Gun::update(Region& region, float timeElapsed)
{
	if (!m_Shooting)
	{
		m_Recoil = 0;
	}

	m_Shooting = false;

	if (m_Light.getUV()[0].z > 50)
	{
		m_Light.setLightPosition(m_Light.getUV()[0].x, m_Light.getUV()[0].y, m_Light.getUV()[0].z - 10);
	}
	else
	{
		m_Light.setLightPosition(m_Light.getUV()[0].x, m_Light.getUV()[0].y, 0);
	}

	float mx = Window::Instance().getMouseWorldPosX();
	float my = Window::Instance().getMouseWorldPosY();

	glm::mat4 transform;

	m_Angle = Utils::calcAngleRad(getCenterX(), getCenterY(), mx, my);
	int rtx = 16;
	int rty = 16;
	transform = glm::translate(transform, glm::vec3(getPosition().x + rtx, getPosition().y + rty, 0));
	transform = glm::rotate(transform, m_Angle, glm::vec3(0, 0, 1));
	transform = glm::translate(transform, glm::vec3(-getPosition().x - 1 + m_Recoil, -getPosition().y - 23, 0));
	const glm::vec3& pos = glm::vec3(getPosition().x, getCenterY(), 0);
	glm::vec4 gunPos = transform * glm::vec4(pos, 1.0f);

	float newAngle = Utils::calcAngleRad(gunPos.x, gunPos.y, Window::Instance().getMouseWorldPosX(), Window::Instance().getMouseWorldPosY());
	m_Angle = newAngle;

	//if (m_FaceRight && (std::abs(glm::degrees(m_Angle)) >= 90.0f))
	//{
	//	addDirection(5, 0);
	//	//setUV(0, 1, 16, 3);
	//	m_FaceRight = false;
	//}
	//else if (!m_FaceRight && (std::abs(glm::degrees(m_Angle)) < 90.0f))
	//{
	//	addDirection(-5, 0);
	//	//setUV(0, 0, 16, 3);
	//	m_FaceRight = true;
	//}

	// bullet collision and updates
	for (auto& bullet : m_Bullets)
	{
		bullet->update(timeElapsed);
		if (bullet->shouldDestroy()) continue;

		std::vector<Entity*> objects;

		float bx = bullet->getCollisionBox().x;
		float by = bullet->getCollisionBox().y;
		float bw = bullet->getCollisionBox().width;
		float bh = bullet->getCollisionBox().height;

		float bdx = bullet->m_Dx * timeElapsed;
		float bdy = bullet->m_Dy * timeElapsed;

		// used a larger bounding box range because some objects were not being retrieved on the edge cases.
		ObjectManager::ObjectsQT->retrieve(objects, BoundingBox(bx - bdx, by - bdy, bdx, bdy));
		//quadTree->retrieve(objects, BoundingBox(std::fminf(bx, bx + bdx) - 32, std::fminf(by, by + bdy) - 32, bw + std::abs(bdx) + 64, bh + std::abs(bdy) + 64));
		for (auto& object : objects)
		{
			const auto& collisionBox = object->getCollisionBox();

			float cx = collisionBox.x;
			float cy = collisionBox.y;
			float cw = collisionBox.width;
			float ch = collisionBox.height;

			if (Utils::lineIntersection(glm::vec4(bx, by, bx - bullet->m_Dx * timeElapsed, by - bullet->m_Dy * timeElapsed), glm::vec4(cx, cy, cx, cy + ch)) ||
				Utils::lineIntersection(glm::vec4(bx, by, bx - bullet->m_Dx * timeElapsed, by - bullet->m_Dy * timeElapsed), glm::vec4(cx, cy, cx + cw, cy)) ||
				Utils::lineIntersection(glm::vec4(bx, by, bx - bullet->m_Dx * timeElapsed, by - bullet->m_Dy * timeElapsed), glm::vec4(cx + cw, cy + ch, cx + cw, cy)) ||
				Utils::lineIntersection(glm::vec4(bx, by, bx - bullet->m_Dx * timeElapsed, by - bullet->m_Dy * timeElapsed), glm::vec4(cx, cy + ch, cx + cw, cy + ch))
				)
			{
				glm::vec2 collision_pos = Utils::intersection(collisionBox, glm::vec4(bx, by, bx - bullet->m_Dx * timeElapsed, by - bullet->m_Dy * timeElapsed));
				bullet->setPosition(collision_pos.x, collision_pos.y);
				bullet->updateLight();
				//float randOffset = Utils::random(-30.0f, 30.0f);
				for (int i = 0; i < 50; i++)
				{
					//m_Entities.push_back(std::unique_ptr<Particle>(new Particle(cx, cy, 5.0f, glm::degrees(bullet->getAngle()))));
					//m_Entities.push_back(std::make_unique<Particle>(cx, cy, Utils::random(5, 20), object->getTexture()));
					//ParticleManager::instance().add(Particle(cx, cy, Utils::random(1, 10), glm::vec4(Utils::random(0.8f, 1.0f), 0, 0, Utils::random(0.4f, 1.0f)), glm::degrees(bullet->getAngle()) + 180.0f + randOffset));
					//ParticleManager::instance().add(Particle(cx, cy, Utils::random(1, 10), glm::vec4(Utils::random(0.8f, 1.0f), 0, 0, Utils::random(0.4f, 1.0f)), glm::degrees(bullet->getAngle()) + 180.0f + Utils::random(-90.0f, 90.0f)));
					m_Entities.push_back(std::make_unique<CollisionParticle>(collision_pos.x, collision_pos.y, glm::degrees(m_Angle)));
					//m_Entities.push_back(std::make_unique<CollisionParticle>(cx, cy, glm::degrees(m_Angle), object->getTexture()));
				}

				//randOffset = Utils::random(-90.0f, 90.0f);
				//for (int i = 0; i < 50; i++)
				//{
				//	ParticleManager::instance().add(Particle(cx, cy, Utils::random(1, 10), glm::vec4(Utils::random(0.8f, 1.0f), 0, 0, Utils::random(0.4f, 1.0f)), glm::degrees(bullet->getAngle()) + 180.0f + randOffset));
				//}

				//m_DamageText.push_back(std::make_unique<DamageCounter>("TEST", cx, cy));

				//region.removeTiles(collisionBox->x, collisionBox->y, true, true);
				object->setAngle(m_Angle);
				object->damage(1);
				//object->setDestroy(true);
				bullet->setDestroy(true);
				region.removeObject(object->getX(), object->getY());
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

	//glEnable(GL_DEPTH_TEST);
	//renderer.m_AlphaTest = false;
	for (auto& bullet : m_Bullets) bullet->submit(renderer);
	
	renderer.end();
	renderer.flush();

	renderer.begin();
	for (auto& entity : m_Entities)
	{
		//glm::mat4 transform;
		//transform = glm::translate(transform, glm::vec3(entity->getCenterX(), entity->getCenterY(), 0));
		//transform = glm::rotate(transform, entity->getAngle(), glm::vec3(0, 0, 1));
		//transform = glm::translate(transform, glm::vec3(-entity->getCenterX(), -entity->getCenterY(), 0));
		//renderer.push(transform);
		entity->submit(renderer);
		//renderer.pop();
	}

	renderer.end();
	ResourceManager::getInstance().shader("basic_shader")->setUniform("outline", true);
	renderer.flush();
	ResourceManager::getInstance().shader("basic_shader")->setUniform("outline", false);
	//renderer.m_AlphaTest = true;
	//glDisable(GL_DEPTH_TEST);

	for (auto& text : m_DamageText) text->render(renderer);

	renderer.pop();

	int rtx = 16;
	int rty = 16;
	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(getPosition().x + rtx, getPosition().y + rty, 0));
	transform = glm::rotate(transform, m_Angle, glm::vec3(0, 0, 1));
	transform = glm::translate(transform, glm::vec3(-getPosition().x - 1 + m_Recoil, -getPosition().y - 23, 0));

	renderer.push(transform);
	renderer.render(*this);
	renderer.pop();

	//for (auto& bullet : m_Bullets)
	//{
	//	float bx = bullet->getCollisionBox()->x;
	//	float by = bullet->getCollisionBox()->y;
	//	float bw = bullet->getCollisionBox()->width;
	//	float bh = bullet->getCollisionBox()->height;

	//	float bdx = bullet->m_Dx * 0.01f;
	//	float bdy = bullet->m_Dy * 0.01f;

	//	// used a larger bounding box range because some objects were not being retrieved on the edge cases.
	//	//BoundingBox bbox(std::fminf(bx, bx + bdx) - 32, std::fminf(by, by + bdy) - 32, bw + std::abs(bdx) + 64, bh + std::abs(bdy) + 64);
	//	BoundingBox bbox(bx - bdx, by - bdy, bdx, bdy);
	//	renderer.debugRender(bbox, TextureManager::get("Textures/collision_box.png"));
	//}
}

void Gun::renderLight(Renderer& renderer)
{
	//m_Light.submit(renderer);
	for (auto& bullet : m_Bullets) bullet->renderLight(renderer);
}
