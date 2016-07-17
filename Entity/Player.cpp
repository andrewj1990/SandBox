#include "player.h"

Player::Player(float x, float y)
	: Entity(x, y), m_Sword(x + 2, y + 8), m_Gun(x + 2, y + 8)
{
	m_Sprite = Sprite(glm::vec3(x, y, 0), glm::vec2(32, 32), TextureManager::get("Textures/Player/PlayerSpritesheet10.png"));
	m_Shield = Sprite(glm::vec3(x, y, 0), glm::vec2(32, 32), TextureManager::get("Textures/Player/Shield.png"));

	init();
}

void Player::init()
{
	m_State = PlayerState::NORMAL;
	m_CumulativeTime = 0.0f;
	m_AttackSpeed = 0.3f;
	m_AttackFrame = 0.0f;
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

	float dx = 0.0f;
	float dy = 0.0f;
	float vel = 96.0f;

	if (window.isKeyPressed(GLFW_KEY_W)) dy += vel;
	if (window.isKeyPressed(GLFW_KEY_A)) dx -= vel;
	if (window.isKeyPressed(GLFW_KEY_S)) dy -= vel;
	if (window.isKeyPressed(GLFW_KEY_D)) dx += vel;

	if (!(terrain.isCollidable(m_Sprite.getPosition().x + dx * timeElapsed, m_Sprite.getPosition().y) ||
		terrain.isCollidable(m_Sprite.getPosition().x + m_Sprite.getSize().x + dx * timeElapsed, m_Sprite.getPosition().y)))
	{
		move(dx * timeElapsed, 0);
	}

	if (!(terrain.isCollidable(m_Sprite.getPosition().x, m_Sprite.getPosition().y + dy * timeElapsed) ||
		terrain.isCollidable(m_Sprite.getPosition().x, m_Sprite.getPosition().y + m_Sprite.getSize().y+ dy * timeElapsed)))
	{
		move(0, dy * timeElapsed);
	}

}

void Player::shoot(float angle, float timeElapsed)
{
	m_AttackFrame += timeElapsed;

	// fire projectile
	if (Window::Instance().isButtonPressed(GLFW_MOUSE_BUTTON_1) && m_AttackFrame > m_AttackSpeed)
	{
		m_Gun.shoot(m_Sprite.getPosition().x, m_Sprite.getPosition().y, angle + glm::radians(30.0f));
		m_Gun.shoot(m_Sprite.getPosition().x, m_Sprite.getPosition().y, angle - glm::radians(30.0f));
		m_Gun.shoot(m_Sprite.getPosition().x, m_Sprite.getPosition().y, angle + glm::radians(20.0f));
		m_Gun.shoot(m_Sprite.getPosition().x, m_Sprite.getPosition().y, angle - glm::radians(20.0f));
		m_Gun.shoot(m_Sprite.getPosition().x, m_Sprite.getPosition().y, angle + glm::radians(10.0f));
		m_Gun.shoot(m_Sprite.getPosition().x, m_Sprite.getPosition().y, angle - glm::radians(10.0f));
		m_Gun.shoot(m_Sprite.getPosition().x, m_Sprite.getPosition().y, angle);

		m_AttackFrame = 0.0f;
	}
}

void Player::move(float dx, float dy)
{
	m_Sprite.addDirection(dx, dy);
	m_Shield.addDirection(dx, dy);
	m_Sword.move(dx, dy);
	m_Gun.move(dx, dy);

	Camera& camera = Window::Instance().getCamera();
	camera.moveCamera(dx, dy);
	ResourceManager::getInstance().shader("basic_shader")->setUniform("view", camera.GetViewMatrix());

}

void Player::dodge()
{
	float dodgeDistance = 2.0f;
	float dx = dodgeDistance * std::cosf(m_DodgeAngle);
	float dy = dodgeDistance * std::sinf(m_DodgeAngle);
	float dodgeDuration = 0.5f;

	dx /= dodgeDuration;
	dy /= dodgeDuration;

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

	float dx = mouseX - m_X;
	float dy = mouseY - m_Y;
	float angle = -std::atan2f(dy, dx);

	if (playerCollision(quadTree))
	{
		//std::cout << "player collision\n";
	}

	update(timeElapsed);
	move(terrain, timeElapsed);
	shoot(angle, timeElapsed);

	m_Sword.update(quadTree, timeElapsed);
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
		dodge();
		break;
	default:
		break;
	}

}

void Player::render(Renderer& renderer)
{
	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(m_Sprite.getPosition().x + m_Sprite.getSize().x / 2.0f, m_Sprite.getPosition().y + m_Sprite.getSize().y / 2.0f, 0));
	transform = glm::rotate(transform, m_Sprite.getAngle(), glm::vec3(0, 0, 1));
	transform = glm::translate(transform, glm::vec3(-m_Sprite.getPosition().x - m_Sprite.getSize().x / 2.0f, -m_Sprite.getPosition().y - m_Sprite.getSize().y / 2.0f, 0));
	renderer.push(transform);
	renderer.render(m_Sprite);
	//m_Sword.render(renderer);
	m_Gun.render(renderer);
	renderer.pop();


	if (m_ShieldActive)
	{
		transform = glm::mat4();
		transform = glm::translate(transform, glm::vec3(m_Shield.getPosition().x + m_Shield.getSize().x / 2.0f, m_Shield.getPosition().y + m_Shield.getSize().y / 2.0f, 0));
		transform = glm::rotate(transform, m_Shield.getAngle(), glm::vec3(0, 0, 1));
		transform = glm::translate(transform, glm::vec3(-m_Shield.getPosition().x - m_Shield.getSize().x / 2.0f, -m_Shield.getPosition().y - m_Shield.getSize().y / 2.0f, 0));
		renderer.push(transform);
		renderer.render(m_Shield);
		renderer.pop();
	}

}
