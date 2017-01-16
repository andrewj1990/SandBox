#include "player.h"

Player::Player(float x, float y)
	: Entity(glm::vec3(x, y, 0), glm::vec2(32, 32), TextureManager::get("Textures/Player/player_anim.png")), 
	m_Sword(x + 2, y + 8), m_Gun(x + 10, y + 12), m_CollisionBox(x, y, 10, 22)
{
	m_Light = Sprite(glm::vec3(0, 0, 0), glm::vec2(256, 256), TextureManager::get("Textures/light2.png"));
	m_Shield = Sprite(glm::vec3(x, y, 0), glm::vec2(32, 32), TextureManager::get("Textures/Player/Shield.png"));
	//m_Crosshair = Sprite(glm::vec3(x, y, 0), glm::vec2(32, 32), TextureManager::get("Textures/Player/crosshair.png"));
	m_TexSize = 16;

	init();
}

void Player::init()
{
	m_State = PlayerState::NORMAL;
	m_Row = 0;
	m_CumulativeTime = 0.0f;
	m_NoClip = false;

	m_MoveSpeed = 280.0f;
	m_AttackSpeed = 0.1f;
	m_AttackFrame = 0.0f;

	m_Moving = false;

	m_Sprite.setUV(1, 0, m_TexSize, m_TexSize);

	Camera& camera = Window::Instance().getCamera();
	camera.Position = glm::vec3(0, 0, 0);
}

bool Player::playerCollision(float dx, float dy, const std::unique_ptr<QTree<BoundingBox>>& quadTree)
{
	//float x = getX() + dx;
	//float y = getY() + dy;
	//float w = getSprite().getSize().x;
	//float h = getSprite().getSize().y;

	float x = m_CollisionBox.x + dx;
	float y = m_CollisionBox.y + dy;
	float w = m_CollisionBox.width;
	float h = m_CollisionBox.height;

	std::vector<std::shared_ptr<BoundingBox>> m_CollisionBoxes;
	//quadTree->retrieve(m_CollisionBoxes, BoundingBox(x, y, getSprite().getSize().x, getSprite().getSize().y));
	quadTree->retrieve(m_CollisionBoxes, m_CollisionBox);

	for (auto& tile : m_CollisionBoxes)
	{
		float tx = tile->x;
		float ty = tile->y;
		float tw = tile->width;
		float th = tile->height;

		//if (tile->intersects(m_CollisionBox))
		if (Utils::quadCollision(x, y, w, h, tx, ty, tw, th))
		{
			return true;
		}
	}

	return false;
}

void Player::move(const std::unique_ptr<QTree<BoundingBox>>& quadTree, float timeElapsed)
{
	Window& window = Window::Instance();

	m_Anim += timeElapsed * 25.0f;

	float dx = 0.0f;
	float dy = 0.0f;//-200.0f * timeElapsed;

	if (!m_Moving)
	{
		m_Sprite.setUV(0, 0, m_TexSize, m_TexSize);
	}
	else
	{
		m_Moving = false;
	}

	if (window.isKeyPressed(GLFW_KEY_W))
	{
		m_Row = 3;
		m_Sprite.setUV((int)m_Anim % 6, m_Row, m_TexSize, m_TexSize);
		dy += m_MoveSpeed * timeElapsed;
		m_Moving = true;
	}

	if (window.isKeyPressed(GLFW_KEY_A))
	{
		m_Row = 2;
		m_Sprite.setUV((int)m_Anim % 8, m_Row, m_TexSize, m_TexSize);
		dx -= m_MoveSpeed * timeElapsed;
		m_Moving = true;
	}

	if (window.isKeyPressed(GLFW_KEY_S))
	{
		m_Row = 3;
		m_Sprite.setUV((int)m_Anim % 6, m_Row, m_TexSize, m_TexSize);
		dy -= m_MoveSpeed * timeElapsed;
		m_Moving = true;
	}

	if (window.isKeyPressed(GLFW_KEY_D))
	{
		m_Row = 1;
		m_Sprite.setUV((int)m_Anim % 8, m_Row, m_TexSize, m_TexSize);
		dx += m_MoveSpeed * timeElapsed;
		m_Moving = true;
	}

	if (!m_NoClip)
	{
		if (playerCollision(dx, 0, quadTree))
		{
			dx = 0.0f;
		}

		if (playerCollision(0, dy, quadTree))
		{
			dy = 0.0f;
		}
	}

	move(dx, dy);

	m_Light.setPosition(getCenterX() - m_Light.getSize().x / 2, getCenterY() - m_Light.getSize().y / 2);
}

void Player::shoot(float angle, float timeElapsed)
{
	m_AttackFrame += timeElapsed;

	// fire projectile
	if (Window::Instance().isButtonPressed(GLFW_MOUSE_BUTTON_1) && m_AttackFrame > m_AttackSpeed)
	{
		//for (int i = 3; i > 0; i--)
		//{
		//	m_Gun.shoot(m_Sprite.getPosition().x, m_Sprite.getPosition().y, angle + glm::radians(10.0f * i));
		//	m_Gun.shoot(m_Sprite.getPosition().x, m_Sprite.getPosition().y, angle - glm::radians(10.0f * i));
		//}

		float angleOffset = glm::radians(Utils::random(-2.5f, 2.5f));
		m_Gun.shoot(m_Sprite.getPosition().x, m_Sprite.getPosition().y, angle + angleOffset, m_Moving ? m_MoveSpeed : 0.0f);

		m_AttackFrame = 0.0f;
	}

	//for (int i = 15; i > 0; i--)
	//{
	//	m_Gun.shoot(m_Sprite.getPosition().x, m_Sprite.getPosition().y, angle + glm::radians(10.0f * i));
	//	m_Gun.shoot(m_Sprite.getPosition().x, m_Sprite.getPosition().y, angle - glm::radians(10.0f * i));
	//}
	//m_Gun.shoot(m_Sprite.getPosition().x, m_Sprite.getPosition().y, angle);
}

void Player::move(float dx, float dy)
{
	m_Sprite.addDirection(dx, dy);
	m_Shield.addDirection(dx, dy);
	m_Sword.move(dx, dy);
	m_Gun.move(dx, dy);
	m_CollisionBox.x = getCenterX() - 5;
	m_CollisionBox.y = m_Sprite.getPosition().y;

	Camera& camera = Window::Instance().getCamera();
	camera.moveCamera(dx, dy);

	ResourceManager::getInstance().shader("outline_shader")->setUniform("view", camera.GetViewMatrix());
	ResourceManager::getInstance().shader("basic_shader")->setUniform("view", camera.GetViewMatrix());
	ResourceManager::getInstance().shader("lightShadow")->setUniform("view", camera.GetViewMatrix());
}

void Player::dodge(const Terrain& terrain)
{
	float dodgeDistance = 2.0f;
	float dx = dodgeDistance * std::cosf(m_DodgeAngle);
	float dy = dodgeDistance * std::sinf(m_DodgeAngle);
	float dodgeDuration = 0.5f;

	dx /= dodgeDuration;
	dy /= dodgeDuration;

	if (terrain.isSolid(m_Sprite.getPosition().x + dx, m_Sprite.getPosition().y) ||
		terrain.isSolid(m_Sprite.getPosition().x + m_Sprite.getSize().x + dx, m_Sprite.getPosition().y) ||
		terrain.isSolid(m_Sprite.getPosition().x + dx, m_Sprite.getPosition().y + m_Sprite.getSize().y) ||
		terrain.isSolid(m_Sprite.getPosition().x + m_Sprite.getSize().x + dx, m_Sprite.getPosition().y + m_Sprite.getSize().y))
	{
		dx = 0.0f;
	}

	if (terrain.isSolid(m_Sprite.getPosition().x, m_Sprite.getPosition().y + dy) ||
		terrain.isSolid(m_Sprite.getPosition().x, m_Sprite.getPosition().y + m_Sprite.getSize().y + dy) ||
		terrain.isSolid(m_Sprite.getPosition().x + m_Sprite.getSize().x, m_Sprite.getPosition().y + dy) ||
		terrain.isSolid(m_Sprite.getPosition().x + m_Sprite.getSize().x, m_Sprite.getPosition().y + m_Sprite.getSize().y + dy))
	{
		dy = 0.0f;
	}

	move(dx, dy);
	
	m_Sprite.setAngle(m_Sprite.getAngle() + glm::radians(-20.0f));

	if (m_CumulativeTime >= dodgeDuration)
	{
		m_State = PlayerState::NORMAL;
		m_Sprite.setAngle(0.0f);
	}
}

void Player::update(Region& region, const std::unique_ptr<QTree<BoundingBox>>& quadTree, float timeElapsed)
{
	update(timeElapsed);
	move(quadTree, timeElapsed);
	m_Gun.update(region, quadTree, timeElapsed);

	Window& win = Window::Instance();
	//m_Crosshair.setPosition(win.getCamera().Position.x + win.mouseX(), win.getCamera().Position.y + (win.getHeight() - win.mouseY()));

	if (Window::Instance().isKeyPressed(GLFW_KEY_O) && m_AttackFrame > 0.5)
	{
		m_NoClip = !m_NoClip;
		m_AttackFrame = 0;
	}

}

void Player::update(float timeElapsed)
{
	Window& window = Window::Instance();
	float mouseX = Window::Instance().mouseX();
	float mouseY = Window::Instance().mouseY();

	//float dx = mouseX - m_X;
	//float dy = mouseY - m_Y;
	//float angle = -std::atan2f(dy, dx);

	float dx = mouseX - Window::Instance().getWidth() / 2 - 16.0f + 2;
	float dy = mouseY - Window::Instance().getHeight() / 2 - 16.0f + 8;
	float angle = -std::atan2f(dy, dx);// -glm::radians(45.0f);

	float mx = Window::Instance().getMouseWorldPosX();
	float my = Window::Instance().getMouseWorldPosY();
	dx = mx - getCenterX();
	dy = my - getCenterY();
	angle = std::atan2f(dy, dx);

	shoot(angle, timeElapsed);
}

void Player::submit(Renderer& renderer)
{
	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(m_Sprite.getPosition().x + m_Sprite.getSize().x / 2.0f, m_Sprite.getPosition().y + m_Sprite.getSize().y / 2.0f, 0));
	transform = glm::rotate(transform, m_Sprite.getAngle(), glm::vec3(0, 0, 1));
	transform = glm::translate(transform, glm::vec3(-m_Sprite.getPosition().x - m_Sprite.getSize().x / 2.0f, -m_Sprite.getPosition().y - m_Sprite.getSize().y / 2.0f, 0));

	if (m_Row == 3)
	{
		m_Gun.submit(renderer);
	}

	renderer.push(transform);
	renderer.end();
	renderer.flush();
	renderer.render(m_Sprite);
	renderer.pop();
	renderer.begin();

	if (m_Row != 3)
	{
		m_Gun.submit(renderer);
	}

	//if (m_ShieldActive)
	//{
	//	transform = glm::mat4();
	//	transform = glm::translate(transform, glm::vec3(m_Shield.getPosition().x + m_Shield.getSize().x / 2.0f, m_Shield.getPosition().y + m_Shield.getSize().y / 2.0f, 0));
	//	transform = glm::rotate(transform, m_Shield.getAngle(), glm::vec3(0, 0, 1));
	//	transform = glm::translate(transform, glm::vec3(-m_Shield.getPosition().x - m_Shield.getSize().x / 2.0f, -m_Shield.getPosition().y - m_Shield.getSize().y / 2.0f, 0));
	//	renderer.push(transform);
	//	renderer.render(m_Shield);
	//	renderer.pop();
	//}

}

void Player::render(Renderer& renderer)
{
	float scale = 1.1f;
	glm::mat4 transform;
	glm::mat4 outline;
	outline = glm::translate(outline, glm::vec3(m_Sprite.getPosition().x + m_Sprite.getSize().x / 2.0f, m_Sprite.getPosition().y + m_Sprite.getSize().y / 2.0f, 0));
	outline = glm::scale(outline, glm::vec3(scale, scale, scale));
	outline = glm::rotate(outline, m_Sprite.getAngle(), glm::vec3(0, 0, 1));
	outline = glm::translate(outline, glm::vec3(-m_Sprite.getPosition().x - m_Sprite.getSize().x / 2.0f, -m_Sprite.getPosition().y - m_Sprite.getSize().y / 2.0f, 0));

	transform = glm::translate(transform, glm::vec3(m_Sprite.getPosition().x + m_Sprite.getSize().x / 2.0f, m_Sprite.getPosition().y + m_Sprite.getSize().y / 2.0f, 0));
	transform = glm::rotate(transform, m_Sprite.getAngle(), glm::vec3(0, 0, 1));
	transform = glm::translate(transform, glm::vec3(-m_Sprite.getPosition().x - m_Sprite.getSize().x / 2.0f, -m_Sprite.getPosition().y - m_Sprite.getSize().y / 2.0f, 0));

	if (m_Gun.getSprite().getAngle() >= 0)
	{
		m_Gun.render(renderer);
	}

	renderer.push(transform);
	renderer.render(m_Sprite);
	renderer.pop();

	//m_Sword.render(renderer);
	if (m_Gun.getSprite().getAngle() < 0)
	{
		m_Gun.render(renderer);
	}

	//if (m_ShieldActive)
	//{
	//	transform = glm::mat4();
	//	transform = glm::translate(transform, glm::vec3(m_Shield.getPosition().x + m_Shield.getSize().x / 2.0f, m_Shield.getPosition().y + m_Shield.getSize().y / 2.0f, 0));
	//	transform = glm::rotate(transform, m_Shield.getAngle(), glm::vec3(0, 0, 1));
	//	transform = glm::translate(transform, glm::vec3(-m_Shield.getPosition().x - m_Shield.getSize().x / 2.0f, -m_Shield.getPosition().y - m_Shield.getSize().y / 2.0f, 0));
	//	renderer.push(transform);
	//	renderer.render(m_Shield);
	//	renderer.pop();
	//}

	ResourceManager::getInstance().shader("basic_shader")->use();
	//renderer.render(m_Crosshair);
	//renderer.render(m_CollisionBox, TextureManager::get("Textures/collision_box.png"));

}

void Player::renderLight(Renderer& renderer)
{
	renderer.submit(m_Light);

	m_Gun.renderLight(renderer);

}
