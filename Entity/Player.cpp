#include "player.h"

Player::Player(float x, float y)
	: Entity(glm::vec3(x, y, 0), glm::vec2(32, 32), TextureManager::get("Textures/Player/Player2.png")), 
	m_Sword(x + 2, y + 8), m_Gun(x + 2, y + 8)
{
	m_Light = Sprite(glm::vec3(0, 0, 0), glm::vec2(256, 256), TextureManager::get("Textures/light2.png"));
	m_Shield = Sprite(glm::vec3(x, y, 0), glm::vec2(32, 32), TextureManager::get("Textures/Player/Shield.png"));

	init();
}

void Player::init()
{
	m_State = PlayerState::NORMAL;
	m_Row = 0;
	m_CumulativeTime = 0.0f;

	m_MoveSpeed = 180.0f;
	m_AttackSpeed = 0.5f;
	m_AttackFrame = 0.0f;

	m_Sprite.setUV(1, 0, 10, 10);

	Camera& camera = Window::Instance().getCamera();
	camera.Position = glm::vec3(0, 0, 0);
}

bool Player::playerCollision(const std::unique_ptr<QuadTree>& quadTree)
{
	std::vector<Renderable*> enemies;
	quadTree->retrieve(enemies, m_Sprite.getPosition().x, m_Sprite.getPosition().y, 10, 10);

	for (auto enemy : enemies)
	{
		float ex = enemy->getPosition().x;
		float ey = enemy->getPosition().y;
		float ew = enemy->getSize().x;
		float eh = enemy->getSize().y;

		float px = m_Sprite.getPosition().x;
		float py = m_Sprite.getPosition().y;
		float pw = m_Sprite.getSize().x;
		float ph = m_Sprite.getSize().y;

		if (px > ex && px < ex + ew && py > ey && py < ey + eh)
		{
			return true;
		}

	}
	return false;
}

void Player::move(const Terrain& terrain, float timeElapsed)
{
	Window& window = Window::Instance();

	m_Anim += timeElapsed * 10.0f;

	float dx = 0.0f;
	float dy = 0.0f;

	if (window.isKeyPressed(GLFW_KEY_W))
	{
		m_Row = 3;
		m_Sprite.setUV((int)m_Anim % 3, m_Row, 10, 10);
		dy += m_MoveSpeed * timeElapsed;
	}

	if (window.isKeyPressed(GLFW_KEY_A))
	{
		m_Row = 2;
		m_Sprite.setUV((int)m_Anim % 3, m_Row, 10, 10);
		dx -= m_MoveSpeed * timeElapsed;
	}

	if (window.isKeyPressed(GLFW_KEY_S))
	{
		m_Row = 0;
		m_Sprite.setUV((int)m_Anim % 3, m_Row, 10, 10);
		dy -= m_MoveSpeed * timeElapsed;
	}

	if (window.isKeyPressed(GLFW_KEY_D))
	{
		m_Row = 1;
		m_Sprite.setUV((int)m_Anim % 3, m_Row, 10, 10);
		dx += m_MoveSpeed * timeElapsed;
	}

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
		m_Gun.shoot(m_Sprite.getPosition().x, m_Sprite.getPosition().y, angle);

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

	Camera& camera = Window::Instance().getCamera();
	camera.moveCamera(dx, dy);
	ResourceManager::getInstance().shader("outline_shader")->setUniform("view", camera.GetViewMatrix());
	ResourceManager::getInstance().shader("basic_shader")->setUniform("view", camera.GetViewMatrix());

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

void Player::update(const Terrain& terrain, const std::unique_ptr<QuadTree>& quadTree, float timeElapsed)
{
	Window& window = Window::Instance();
	float mouseX = Window::Instance().mouseX();
	float mouseY = Window::Instance().mouseY();

	float dx = mouseX - (window.getWidth() / 2 - 16.0f);
	float dy = mouseY - (window.getHeight() / 2 - 16.0f);
	float angle = -std::atan2f(dy, dx);

	m_CumulativeTime += timeElapsed;

	if (playerCollision(quadTree))
	{
		//std::cout << "player collision\n";
	}

	switch (m_State)
	{
	case (PlayerState::ATTACK) :
		break;
	case (PlayerState::NORMAL) :
		// shield
		m_Shield.setAngle(angle);
		m_ShieldActive = window.isButtonPressed(GLFW_MOUSE_BUTTON_2);

		// sword
		if (window.isButtonPressed(GLFW_MOUSE_BUTTON_1)) m_Sword.setAttackParams(angle);

		// movement

		if (window.isKeyPressed(GLFW_KEY_SPACE))
		{
			m_State = PlayerState::DODGE;
			m_CumulativeTime = 0.0f;
			m_DodgeAngle = angle;
		}
		break;
	case (PlayerState::DODGE) :
		dodge(terrain);
		break;
	default:
		break;
	}

	move(terrain, timeElapsed);
	shoot(angle, timeElapsed);

	//m_Sword.update(quadTree, timeElapsed);
	m_Gun.update(quadTree, timeElapsed);
}

void Player::update(float timeElapsed)
{
	Window& window = Window::Instance();
	float mouseX = Window::Instance().mouseX();
	float mouseY = Window::Instance().mouseY();

	float dx = mouseX - m_X;
	float dy = mouseY - m_Y;
	float angle = -std::atan2f(dy, dx);

	m_CumulativeTime += timeElapsed;

	switch (m_State)
	{
	case (PlayerState::ATTACK) :
		break;
	case (PlayerState::NORMAL) :
		// shield
		m_Shield.setAngle(angle);
		m_ShieldActive = window.isButtonPressed(GLFW_MOUSE_BUTTON_2);

		// sword
		if (window.isButtonPressed(GLFW_MOUSE_BUTTON_1)) m_Sword.setAttackParams(angle);

		// movement
		
		if (window.isKeyPressed(GLFW_KEY_SPACE))
		{
			m_State = PlayerState::DODGE;
			m_CumulativeTime = 0.0f;
			m_DodgeAngle = angle;
		}
		break;
	case (PlayerState::DODGE) :
		//dodge();
		break;
	default:
		break;
	}

	m_Anim += timeElapsed * 10.0f;

	dx = 0.0f;
	dy = 0.0f;

	if (window.isKeyPressed(GLFW_KEY_LEFT_SHIFT))
	{
		m_MoveSpeed += 100.0f;
	}

	if (window.isKeyPressed(GLFW_KEY_W))
	{
		m_Row = 3;
		m_Sprite.setUV((int)m_Anim % 3, m_Row, 10, 10);
		dy += m_MoveSpeed * timeElapsed;
	}

	if (window.isKeyPressed(GLFW_KEY_A))
	{
		m_Row = 2;
		m_Sprite.setUV((int)m_Anim % 3, m_Row, 10, 10);
		dx -= m_MoveSpeed * timeElapsed;
	}

	if (window.isKeyPressed(GLFW_KEY_S))
	{
		m_Row = 0;
		m_Sprite.setUV((int)m_Anim % 3, m_Row, 10, 10);
		dy -= m_MoveSpeed * timeElapsed;
	}

	if (window.isKeyPressed(GLFW_KEY_D))
	{
		m_Row = 1;
		m_Sprite.setUV((int)m_Anim % 3, m_Row, 10, 10);
		dx += m_MoveSpeed * timeElapsed;
	}

	move(dx, dy);
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

	if (m_Row == 3)
	{
		m_Gun.render(renderer);
	}

	renderer.push(transform);
	renderer.render(m_Sprite);
	renderer.pop();

	//m_Sword.render(renderer);
	if (m_Row != 3)
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

}

void Player::renderLight(Renderer& renderer)
{
	renderer.submit(m_Light);

	m_Gun.renderLight(renderer);

}
