
#include <iostream>

#include "Graphics\Window.h"
#include "Graphics\Renderer.h"
#include "Graphics\Buffers\framebuffer.h"

#include "Utils\Shader.h"
#include "Utils\ResourceManager.h"
#include "Utils\Camera.h"
#include "SkeletalAnimation\Skeleton.h"
#include "Entity\player.h"
#include "Level\Level2D.h"
#include "UI\PlayerUI.h"

#include "Graphics\label.h"
#include "Utils\timer.h"
#include "Settings.h"
#include "Graphics\texture_manager.h"

void movement(const Window& window, Camera& camera, float deltaTime)
{
	// Camera controls
	if (window.isKeyPressed(GLFW_KEY_W))
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (window.isKeyPressed(GLFW_KEY_S))
		camera.ProcessKeyboard(UP, deltaTime);
	if (window.isKeyPressed(GLFW_KEY_A))
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (window.isKeyPressed(GLFW_KEY_D))
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

int main()
{
	Window::Instance().setParams("SandBox", 1280, 720);
	//Window::Instance().setParams("SandBox", 1920, 1080);

	ResourceManager::getInstance().addShader("outline_shader", "Shaders/outline.vs", "Shaders/outline.frag");
	ResourceManager::getInstance().addShader("screen", "Shaders/screen.vs", "Shaders/screen.frag");
	ResourceManager::getInstance().addShader("light", "Shaders/light.vs", "Shaders/light.frag");
	ResourceManager::getInstance().addShader("lightShadow", "Shaders/lightShadow.vs", "Shaders/lightshadow.frag");
	ResourceManager::getInstance().addShader("ui", "Shaders/ui.vs", "Shaders/ui.frag");

	ResourceManager::getInstance().shader("basic_shader")->use();
	Renderer batchrenderer;
	
	GLint texID[32];
	for (int i = 0; i < 32; ++i)
	{
		texID[i] = i;
	}
	ResourceManager::getInstance().shader("basic_shader")->setUniform("textures", 32, texID);
	ResourceManager::getInstance().shader("lightShadow")->setUniform("textures", 32, texID);
	ResourceManager::getInstance().shader("ui")->setUniform("textures", 32, texID);
	ResourceManager::getInstance().shader("light")->setUniform("screenTexture", 0);
	ResourceManager::getInstance().shader("light")->setUniform("lightMap", 1);

	Level2D* level2d = new Level2D();
	PlayerUI playerUI(level2d->getPlayerPtr());


	Renderable background(glm::vec3(0,0,0), glm::vec2(700, 700), glm::vec4(1,1,1,1));
	std::vector<Renderable> sprites;

	FontManager::add(new Font("Arial", "arial.ttf", 30));
	Label label("Text test!", 0, 0, FontManager::get("Arial"), glm::vec4(1, 1, 1, 1));

	glm::mat4 projection = glm::ortho(0.0f, Settings::Instance().PROJECTION_WIDTH, 0.0f, Settings::Instance().PROJECTION_HEIGHT, -1.0f, 1.0f);
	ResourceManager::getInstance().shader("basic_shader")->setUniform("projection", projection);
	ResourceManager::getInstance().shader("lightShadow")->setUniform("projection", projection);
	ResourceManager::getInstance().shader("outline_shader")->setUniform("projection", projection);

	FrameBuffer* fbo = new FrameBuffer();
	std::unique_ptr<FrameBuffer> lightFBO = std::unique_ptr<FrameBuffer>(new FrameBuffer());
	std::unique_ptr<FrameBuffer> shadowFBO = std::unique_ptr<FrameBuffer>(new FrameBuffer());
	std::unique_ptr<FrameBuffer> bufferFBO = std::unique_ptr<FrameBuffer>(new FrameBuffer());

	float lightIntensity = 0.3f;
	float ambientIntensity = 0.6f;

	float a = 1280.0f;
	float b = 720.0f;
	float sa = 0.0f;
	float sb = 0.0f;
	float zoom = 0.0f;

	Timer time;
	int frames = 0;
	int updates = 0;
	float updateTimer = 0.0f;
	float tick = 1.0f / 60.0f;
	float t = 0.0f;
	float dt = 0.01f;
	float accumulator = 0.0f;
	int prevDrawCount = 0;
	float timeModifier = 1.0f;
	while (!Window::Instance().shouldClose())
	{
		Settings::Instance().drawCount = 0;
		Settings::Instance().settingsToggle();

		if (Window::Instance().isKeyPressed(GLFW_KEY_N))
		{
			//Level newLevel;
			//level = newLevel;
			//delete level;
			//level = new Level();
		}

		if (Window::Instance().isKeyPressed(GLFW_KEY_K))
		{
			lightIntensity += 0.01f;
			ambientIntensity -= 0.01f;
		}
		if (Window::Instance().isKeyPressed(GLFW_KEY_L))
		{
			lightIntensity -= 0.01f;
			ambientIntensity += 0.01f;
		}

		lightIntensity = lightIntensity < 0 ? 0 : lightIntensity > 1 ? 1 : lightIntensity;
		ambientIntensity = ambientIntensity < 0.3f ? 0.3f : ambientIntensity > 1 ? 1 : ambientIntensity;

		if (Window::Instance().isWindowResized())
		{
			Window::Instance().setWindowResized(false);
			std::cout << "window resized \n";
			delete fbo;
			//delete lightFBO;

			fbo = new FrameBuffer();
			lightFBO = std::unique_ptr<FrameBuffer>(new FrameBuffer());
			shadowFBO = std::unique_ptr<FrameBuffer>(new FrameBuffer());
			//shadowFBO = new FrameBuffer();
		}

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (Window::Instance().isKeyPressed(GLFW_KEY_ESCAPE))
		{
			Window::Instance().closeWindow();
		}

		if (Window::Instance().isKeyTyped(GLFW_KEY_J))
		{
			std::cout << "key j pressed in main\n";
		}

		// zoom
		if (Window::Instance().isKeyPressed(GLFW_KEY_Z))
		{
			Window::Instance().getCamera().processZoom(0.0005);
		}
		if (Window::Instance().isKeyPressed(GLFW_KEY_X))
		{
			Window::Instance().getCamera().processZoom(-0.5);
		}

		float mx = Window::Instance().mouseX();
		float my = Window::Instance().mouseY();

		sa = mx - Settings::Instance().PROJECTION_WIDTH / 2.0f * (1.0f - Window::Instance().getCamera().Zoom);
		sb = Settings::Instance().PROJECTION_HEIGHT - my - Settings::Instance().PROJECTION_HEIGHT / 2.0f * (1.0f - Window::Instance().getCamera().Zoom);
		a = Settings::Instance().PROJECTION_WIDTH * (1.0f - Window::Instance().getCamera().Zoom);
		b = Settings::Instance().PROJECTION_HEIGHT * (1.0f - Window::Instance().getCamera().Zoom);

		if (sa < 0) sa = 0.0f;
		if (sb < 0) sb = 0.0f;
		if (sa + a > Settings::Instance().PROJECTION_WIDTH) sa = Settings::Instance().PROJECTION_WIDTH - a;
		if (sb + b > Settings::Instance().PROJECTION_HEIGHT) sb = Settings::Instance().PROJECTION_HEIGHT - b;

		projection = Window::Instance().getCamera().getProjectionMatrix(sa, sb, a, b);
		ResourceManager::getInstance().shader("basic_shader")->setUniform("projection", projection);
		ResourceManager::getInstance().shader("lightShadow")->setUniform("projection", projection);
		ResourceManager::getInstance().shader("outline_shader")->setUniform("projection", projection);
		
		float frameTime = time.elapsed();
		time.reset();
		if (frameTime > 0.25)
		{
			frameTime = 0.25;
		}

		accumulator += frameTime;

		if (Window::Instance().isKeyPressed(GLFW_KEY_V))
		{
			timeModifier += 0.01f;
			std::cout << "timeSlow : " << timeModifier << "\n";
		}
		if (Window::Instance().isKeyPressed(GLFW_KEY_C))
		{
			timeModifier -= 0.01f;
			std::cout << "timeSlow : " << timeModifier << "\n";
		}

		if (timeModifier <= 0) timeModifier = 0;


		while (accumulator >= dt)
		{
			level2d->update(dt * timeModifier);
			++updates;
			updateTimer += tick;

			t += dt;
			accumulator -= dt;
		}

		fbo->bind();
		ResourceManager::getInstance().shader("basic_shader")->use();
		level2d->render(batchrenderer);

		batchrenderer.render(label);
		fbo->unbind();

		lightFBO->bind();
		level2d->renderLights(batchrenderer);
		lightFBO->unbind();

		ResourceManager::getInstance().shader("light")->use();
		ResourceManager::getInstance().shader("light")->setUniform("lightIntensity", lightIntensity);
		ResourceManager::getInstance().shader("light")->setUniform("ambientIntensity", ambientIntensity);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fbo->getTID());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, lightFBO->getTID());

		glBindVertexArray(fbo->getVAO());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		projection = glm::ortho(0.0f, Settings::Instance().PROJECTION_WIDTH, 0.0f, Settings::Instance().PROJECTION_HEIGHT, -1.0f, 1.0f);

		ResourceManager::getInstance().shader("ui")->setUniform("projection", projection);
		ResourceManager::getInstance().shader("ui")->use();
		playerUI.update();
		playerUI.render(batchrenderer);

		Window::Instance().update();

		++frames;
		if (t > 1.0f) {
			t -= 1.0f;
			printf("%d fps , %d ups\n", frames, updates);
			frames = 0;
			updates = 0;
		}
		if (prevDrawCount != Settings::Instance().drawCount)
		{
			prevDrawCount = Settings::Instance().drawCount;
			std::cout << Settings::Instance().drawCount << "\n";
		}

	}

	return 0;
}