#include "Level2D.h"

Level2D::Level2D()
	: m_Light(), m_Background(glm::vec3(0, 0, 0), glm::vec2(Window::Instance().getWidth(), Window::Instance().getHeight()), TextureManager::get("Textures/Level/bg.png")), m_Region()
{
	int camX = Window::Instance().getCamera().Position.x;
	int camY = Window::Instance().getCamera().Position.y;
	int winW = Window::Instance().getWidth();
	int winH = Window::Instance().getHeight();
	m_QTree = std::unique_ptr<QTree<Renderable>>(new QTree<Renderable>(0, BoundingBox(camX, camY, winW, winH)));

	init();

	m_Delay = 0;
}

void Level2D::init()
{
	const Camera& cam = Window::Instance().getCamera();
	m_Player = std::unique_ptr<Player>(new Player(Window::Instance().getWidth() / 2 - 16.0f, Window::Instance().getHeight() / 2 - 16.0f));

	// move player to surface
	while (true)
	{
		if (m_Region.getSurfacePosition(m_Player->getX(), m_Player->getY()))
		{
			m_Player->move(0, Settings::TILE_SIZE);
			m_Region.update(0);
		}
		else
		{
			m_Player->move(0, Settings::TILE_SIZE);
			m_Region.update(0);
			break;
		}
	}
}

void Level2D::update(float timeElapsed)
{
	Camera& cam = Window::Instance().getCamera();

	m_Background.setPosition(cam.Position.x, cam.Position.y);
	m_Region.update(timeElapsed);
	//m_Player->update(timeElapsed);

	int camX = Window::Instance().getCamera().Position.x;
	int camY = Window::Instance().getCamera().Position.y;
	int winW = Window::Instance().getWidth();
	int winH = Window::Instance().getHeight();
	m_Light.update(m_Player->getCenterX(), m_Player->getCenterY(), timeElapsed);

	m_Delay++;
	if (Window::Instance().isButtonPressed(GLFW_MOUSE_BUTTON_2) && m_Delay > 100)
	{
		m_Delay = 0;
		m_Lights.push_back(Light(m_Light));
	}

	m_QTree = std::unique_ptr<QTree<Renderable>>(new QTree<Renderable>(0, BoundingBox(camX, camY, winW, winH)));
	m_Region.addTiles(m_QTree);

	std::vector<std::shared_ptr<Renderable>> m_Data;
	m_QTree->retrieve(m_Data, m_Light.getLightRegion());

	m_Player->update(m_Region, m_QTree, timeElapsed);
	m_Light.update(m_Data, timeElapsed);

	// move camera based on player position and mouse
	float px = m_Player->getCenterX();
	float py = m_Player->getCenterY();
	float mx = cam.getPosition().x + Window::Instance().mouseX();
	float my = cam.getPosition().y + Window::Instance().mouseY();

	float mcw = 500.0f;
	float mch = 500.0f;
	float dcx = mx - px;
	float dcy = my - py;

	//float cx = mcw < std::abs(dcx) ? mcw : dcx;
	//float cy = mch < std::abs(dcy) ? mch : dcy;

	float cx = std::max(-mcw, std::min(dcx, mcw));
	float cy = -std::max(-mch, std::min(dcy, mch));

	cx /= (mcw / 100.0f);
	cy /= (mcw / 100.0f);

	//float ccx = px - Window::Instance().getWidth() / 2.0f;
	//float ccy = py - Window::Instance().getHeight() / 2.0f;
	float ccx = px - Window::Instance().getInitWidth() / 2.0f;
	float ccy = py - Window::Instance().getInitHeight() / 2.0f;

	cam.moveCameraPosition(ccx + cx, ccy + cy);
	//cam.moveCamera(cx, cy);
}

void Level2D::render(Renderer& renderer)
{
	renderer.render(m_Background);

	m_Region.render(renderer);
	m_Player->render(renderer);
}

void Level2D::renderLights(Renderer& renderer)
{
	ResourceManager::getInstance().shader("lightShadow")->use();

	renderer.begin();
	m_Light.renderShadow(renderer);

	for (auto& light : m_Lights)
	{
		light.renderShadow(renderer);
	}

	renderer.end();
	renderer.flush(GL_SRC_ALPHA, GL_ONE);

	ResourceManager::getInstance().shader("basic_shader")->use();
}
