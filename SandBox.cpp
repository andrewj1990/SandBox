
#include <iostream>

#include "Graphics\Window.h"
#include "Graphics\Renderer.h"
#include "Graphics\Buffers\framebuffer.h"

#include "Utils\Shader.h"
#include "Utils\ResourceManager.h"
#include "Utils\Camera.h"
#include "SkeletalAnimation\Skeleton.h"
#include "Entity\player.h"
#include "Level\Level.h"
#include "Level\Level2D.h"
#include "UI\PlayerUI.h"

#include "Graphics\label.h"
#include "Utils\timer.h"

#define LEVEL 0

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

	ResourceManager::getInstance().addShader("outline_shader", "Shaders/outline.vs", "Shaders/outline.frag");
	ResourceManager::getInstance().addShader("screen", "Shaders/screen.vs", "Shaders/screen.frag");
	ResourceManager::getInstance().addShader("light", "Shaders/light.vs", "Shaders/light.frag");

	ResourceManager::getInstance().shader("basic_shader")->use();
	Renderer batchrenderer;
	
	GLint texID[32];
	for (int i = 0; i < 32; ++i)
	{
		texID[i] = i;
	}
	ResourceManager::getInstance().shader("basic_shader")->setUniform("textures", 32, texID);
	ResourceManager::getInstance().shader("light")->setUniform("screenTexture", 0);
	ResourceManager::getInstance().shader("light")->setUniform("lightMap", 1);
	ResourceManager::getInstance().shader("light")->setUniform("shadowMap", 2);

	Level* level = new Level();
	PlayerUI playerUI(level->getPlayerPtr());

	Level2D* level2d = new Level2D();

	Renderable background(glm::vec3(0,0,0), glm::vec2(700, 700), glm::vec4(1,1,1,1));
	std::vector<Renderable> sprites;

	FontManager::add(new Font("Arial", "arial.ttf", 30));
	Label label("Altitude : ", 0, 0, FontManager::get("Arial"), glm::vec4(1, 1, 1, 1));
	
	//glEnable(GL_DEPTH_TEST);

	glm::mat4 projection = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);
	//glm::mat4 projection = glm::ortho(520.0f, 760.0f, 240.0f, 480.0f, -1.0f, 1.0f);
	ResourceManager::getInstance().shader("basic_shader")->setUniform("projection", projection);
	ResourceManager::getInstance().shader("outline_shader")->setUniform("projection", projection);

	FrameBuffer* fbo = new FrameBuffer();
	FrameBuffer* lightFBO = new FrameBuffer();
	FrameBuffer* shadowFBO = new FrameBuffer();

	float lightIntensity = 0.0f;//1.0f;
	float ambientIntensity = 1.0f;//0.0f;

	Timer time;
	int frames = 0;
	int updates = 0;
	float updateTimer = 0.0f;
	float tick = 1.0f / 60.0f;
	float t = 0.0f;
	float dt = 0.01f;
	float accumulator = 0.0f;
	while (!Window::Instance().shouldClose())
	{
		if (Window::Instance().isKeyPressed(GLFW_KEY_N))
		{
			//Level newLevel;
			//level = newLevel;
			delete level;
			level = new Level();

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
			delete lightFBO;

			fbo = new FrameBuffer();
			lightFBO = new FrameBuffer();
			shadowFBO = new FrameBuffer();
		}

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT/* | GL_DEPTH_BUFFER_BIT*/);

		if (Window::Instance().isKeyPressed(GLFW_KEY_ESCAPE))
		{
			Window::Instance().closeWindow();
		}
		
		float frameTime = time.elapsed();
		time.reset();
		if (frameTime > 0.25)
		{
			frameTime = 0.25;
		}

		accumulator += frameTime;

		while (accumulator >= dt)
		{
			if (LEVEL) level->update(dt);
			else level2d->update(dt);
			playerUI.update(dt);
			++updates;
			updateTimer += tick;

			t += dt;
			accumulator -= dt;
		}

		fbo->bind();
		if (LEVEL) level->render(batchrenderer);
		else level2d->render(batchrenderer);

		batchrenderer.render(label);
		fbo->unbind();

		//fbo.render();

		lightFBO->bind();
		//level->renderLights(batchrenderer);
		level2d->renderLights(batchrenderer);
		lightFBO->unbind();

		shadowFBO->bind();
		level2d->renderShadow(batchrenderer);
		shadowFBO->unbind();

		//lightFBO.render();

		ResourceManager::getInstance().shader("light")->use();
		ResourceManager::getInstance().shader("light")->setUniform("lightIntensity", lightIntensity);
		ResourceManager::getInstance().shader("light")->setUniform("ambientIntensity", ambientIntensity);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fbo->getTID());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, lightFBO->getTID());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, shadowFBO->getTID());

		glBindVertexArray(fbo->getVAO());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		ResourceManager::getInstance().shader("basic_shader")->use();
		playerUI.render(batchrenderer);
		Window::Instance().update();

		++frames;
		if (t > 1.0f) {
			t -= 1.0f;
			printf("%d fps , %d ups\n", frames, updates);
			frames = 0;
			updates = 0;
		}
	}

	return 0;
}


#if 0
#include <iostream>
#include <cmath>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Utils\Shader.h"
#include "Utils\Camera.h"
#include "Utils\LoadImage.h"

#include "Graphics\Model.h"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement();
void renderScene(Shader shader, glm::vec3 cubePositions[]);
GLuint loadCubemap(std::vector<std::string> faces);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool    keys[1024];

// Light attributes
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

GLuint planeVAO;
GLuint containerVAO;

void testFunc(int& test)
{
	int& t = test;
	std::cout << "30 : " << t << "\n";
	std::cout << "30 : " << test << "\n";
	t = 10;
	std::cout << "10 : " << t << "\n";
	std::cout << "30 : " << test << "\n";
}

							// The MAIN function, from here we start the application and run the game loop
int main()
{
	int test = 30;
	testFunc(test);
	std::cout << "30 : " << test << "\n";

	FreeImage_Initialise();
	// Init GLFW
	glfwInit();


	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	// Build and compile our shader program
	Shader lightingShader("Shaders/vertShader.vert", "Shaders/fragShader.frag");
	Shader lampShader("Shaders/lampShader.vert", "Shaders/lampShader.frag");
	Shader basicShader("Shaders/basic.vert", "Shaders/basic.frag");
	Shader skyboxShader("Shaders/skybox.vert", "Shaders/skybox.frag");
	Shader simpleShader("Shaders/simple.vs", "Shaders/simple.frag", "Shaders/simple.geom");
	Shader instancedShader("Shaders/instanced.vs", "Shaders/instanced.frag");
	Shader depthShader("Shaders/depthMap.vs", "Shaders/depthMap.frag");
	Shader depthCubeShader("Shaders/depthCubemap.vs", "Shaders/depthCubemap.frag", "Shaders/depthCubemap.gs");

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// positions		  // normals		  // tc
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
	};

	GLfloat skyboxVertices[] = {
		// Positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	GLfloat tranparentVertices[] = 
	{
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,	0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f,	1.0f, 1.0f,

		 0.5f,  0.5f, 0.0f,	1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,	1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f
	};

	GLfloat quadVertices[] = {   // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
								 // Positions   // TexCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	GLfloat planeVertices[] = 
	{
		-10.0f,  10.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		-10.0f, -10.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		 10.0f,  10.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		 10.0f,  10.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		-10.0f, -10.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		 10.0f, -10.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f
	};

	GLfloat simpleVertices[] =
	{
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
	 	 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // Bottom-left
	};

	GLfloat instancedVertices[] =
	{
		-0.05f,  0.05f, 1.0f, 0.0f, 0.0f, // Top-left
		-0.05f, -0.05f, 1.0f, 1.0f, 0.0f, // Bottom-left
		 0.05f,  0.05f, 0.0f, 1.0f, 0.0f, // Top-right
		 0.05f,  0.05f, 0.0f, 1.0f, 0.0f, // Top-right
		-0.05f, -0.05f, 1.0f, 1.0f, 0.0f, // Bottom-left
		 0.05f, -0.05f, 0.0f, 0.0f, 1.0f  // Bottom-right
	};


	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	std::vector<glm::vec3> vegetation;
	vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	vegetation.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
	vegetation.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

	std::vector<std::string> faces;
	faces.push_back("Textures/right.jpg");
	faces.push_back("Textures/left.jpg");
	faces.push_back("Textures/bottom.jpg");
	faces.push_back("Textures/top.jpg");
	faces.push_back("Textures/back.jpg");
	faces.push_back("Textures/front.jpg");

	glm::vec2 translations[100];
	int index = 0;
	GLfloat offset = 0.1f;
	for (GLint y = -10; y < 10; y += 2)
	{
		for (GLint x = -10; x < 10; x += 2)
		{
			glm::vec2 translation;
			translation.x = (GLfloat)x / 10.0f + offset;
			translation.y = (GLfloat)y / 10.0f + offset;
			translations[index++] = translation;
		}
	}

	GLuint amount = 100000;
	glm::mat4* modelMatrices;
	modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime());
	GLfloat radius = 60.0f;
	offset = 50.0f;
	for (GLuint i = 0; i < amount; i++)
	{
		glm::mat4 model;
		GLfloat angle = (GLfloat)i / (GLfloat)amount * 360.0f;
		GLfloat displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat x = sin(angle) * radius + displacement;
		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat y = displacement * 0.4f; // y value has smaller displacement
		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));
		// 2. Scale: Scale between 0.05 and 0.25f
		GLfloat scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));
		// 3. Rotation: add random rotation around a (semi)randomly picked rotation axis vector
		GLfloat rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
		// 4. Now add to list of matrices
		modelMatrices[i] = model;
	}

	// First, set the container's VAO (and VBO)
	GLuint VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &containerVAO);
	// Position attribute
	glBindVertexArray(containerVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint transparentVAO;
	glGenVertexArrays(1, &transparentVAO);
	glBindVertexArray(transparentVAO);
	GLuint transVBO;
	glGenBuffers(1, &transVBO);
	glBindBuffer(GL_ARRAY_BUFFER, transVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tranparentVertices), tranparentVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);

	GLuint quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glBindVertexArray(0);


	GLuint planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	GLuint skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	GLuint simpleVAO, simpleVBO;
	glGenVertexArrays(1, &simpleVAO);
	glGenBuffers(1, &simpleVBO);
	glBindVertexArray(simpleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, simpleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(simpleVertices), &simpleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	GLuint instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint instancedVAO, instancedVBO;
	glGenVertexArrays(1, &instancedVAO);
	glGenBuffers(1, &instancedVBO);
	glBindVertexArray(instancedVAO);
	glBindBuffer(GL_ARRAY_BUFFER, instancedVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(instancedVertices), &instancedVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(2, 1);
	glBindVertexArray(0);

	unsigned int image_width;
	unsigned int image_height;
	GLubyte* image = loadImage("Textures/container2.png", image_width, image_height);

	GLuint diffuseMap;
	glGenTextures(1, &diffuseMap);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	delete image;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint specularMap;
	glGenTextures(1, &specularMap);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	image = loadImage("Textures/container2_specular.png", image_width, image_height);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	delete image;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint emissionMap;
	glGenTextures(1, &emissionMap);
	glBindTexture(GL_TEXTURE_2D, emissionMap);
	image = loadImage("Textures/matrix.jpg", image_width, image_height);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	delete image;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint transparentTexture;
	glGenTextures(1, &transparentTexture);
	glBindTexture(GL_TEXTURE_2D, transparentTexture);
	image = loadImage("Textures/Bullet.png", image_width, image_height);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	delete image;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint cubemapTexture = loadCubemap(faces);

	// Set texture units
	lightingShader.setUniform("material.diffuse", 0);
	lightingShader.setUniform("material.specular", 1);
	//lightingShader.setUniform("material.emission", 2);
	lightingShader.setUniform("skybox", 2);
	lightingShader.setUniform("shadowMap", 3);
	lightingShader.setUniform("depthMap", 4);

	lampShader.setUniform("tex", 0);

	lightingShader.setUniform("light.direction", -0.2f, -1.0f, -0.3f);

	//Model ourModel("Models/nanosuit.obj");
	//Model rock("Models/Rock/rock.obj");
	//Model planet("Models/Planet/planet.obj");

	//instancedShader.setUniform("texture_diffuse1", 0);

	//glEnable(GL_STENCIL_TEST);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_PROGRAM_POINT_SIZE);

	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	GLuint texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << "\n";
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Set projection matrix
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 1.0f, 1000.0f);
	instancedShader.use();
	glUniformMatrix4fv(glGetUniformLocation(instancedShader.getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	instancedShader.use();
	for (GLuint i = 0; i < 100; i++)
	{
		instancedShader.setUniform(("offsets[" + std::to_string(i) + "]").c_str(), translations[i]);
	}

	//for (int i = 0; i < rock.m_Meshes.size(); i++)
	//{
	//	GLuint vao = rock.m_Meshes[i].m_VAO;
	//	GLuint buffer;
	//	glBindVertexArray(vao);
	//	glGenBuffers(1, &buffer);
	//	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	//	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	//	GLsizei vec4size = sizeof(glm::vec4);
	//	glEnableVertexAttribArray(3);
	//	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (GLvoid*)0);
	//	glEnableVertexAttribArray(4);
	//	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (GLvoid*)(vec4size));
	//	glEnableVertexAttribArray(5);
	//	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (GLvoid*)(2 * vec4size));
	//	glEnableVertexAttribArray(6);
	//	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (GLvoid*)(3 * vec4size));
	//	
	//	glVertexAttribDivisor(3, 1);
	//	glVertexAttribDivisor(4, 1);
	//	glVertexAttribDivisor(5, 1);
	//	glVertexAttribDivisor(6, 1);
	//	
	//	glBindVertexArray(0);
	//}

	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLuint depthCubemap;
	glGenTextures(1, &depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	GLfloat aspect = (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT;
	GLfloat near = 1.0f;
	GLfloat far = 25.0f;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);

	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj * glm::lookAt(pointLightPositions[0], pointLightPositions[0] + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(pointLightPositions[0], pointLightPositions[0] + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(pointLightPositions[0], pointLightPositions[0] + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(pointLightPositions[0], pointLightPositions[0] + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(pointLightPositions[0], pointLightPositions[0] + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(pointLightPositions[0], pointLightPositions[0] + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		do_movement();
		
		// Create camera transformations
		glm::mat4 view;
		glm::mat4 projection;
		//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_FRAMEBUFFER_SRGB);

		// Clear the colorbuffer
		glEnable(GL_DEPTH_TEST);

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.use();
		lightingShader.setUniform("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
		lightingShader.setUniform("dirLight.ambient", 0.2f, 0.2f, 0.2f);
		lightingShader.setUniform("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
		lightingShader.setUniform("dirLight.specular", 1.0f, 1.0f, 1.0f);
		// Point light 1
	
		// wireframe
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		for (int i = 0; i < 4; i++)
		{
			lightingShader.setUniform("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
			lightingShader.setUniform("pointLights[" + std::to_string(i) + "].ambient", 0.05f, 0.05f, 0.05f);
			lightingShader.setUniform("pointLights[" + std::to_string(i) + "].diffuse", 0.8f, 0.8f, 0.8f);
			lightingShader.setUniform("pointLights[" + std::to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);
			lightingShader.setUniform("pointLights[" + std::to_string(i) + "].constant", 1.0f);
			lightingShader.setUniform("pointLights[" + std::to_string(i) + "].linear", 0.09f);
			lightingShader.setUniform("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
		}

		lightingShader.setUniform("spotLight.position", camera.Position.x, camera.Position.y, camera.Position.z);
		lightingShader.setUniform("spotLight.direction", camera.Front.x, camera.Front.y, camera.Front.z);
		lightingShader.setUniform("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		lightingShader.setUniform("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
		lightingShader.setUniform("spotLight.ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setUniform("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setUniform("spotLight.specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setUniform("spotLight.constant", 1.0f);
		lightingShader.setUniform("spotLight.linear", 0.09f);
		lightingShader.setUniform("spotLight.quadratic", 0.032f);

		lightingShader.setUniform("material.shininess", 128.0f);


		// Get the uniform locations
		view = camera.GetViewMatrix();
		projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		// Pass the matrices to the shader
		lightingShader.setUniform("view", view);
		lightingShader.setUniform("projection", projection);

		lightingShader.use();
		//glActiveTexture(GL_TEXTURE2);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		//glActiveTexture(GL_TEXTURE2);
		//glBindTexture(GL_TEXTURE_2D, emissionMap);

		// draw containers and fill the stencil buffer with 1
		//glStencilFunc(GL_ALWAYS, 1, 0xFF);
		//glStencilMask(0xFF);	// enable writes

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		//GLfloat near_plane = 1.0f, far_plane = 7.5f;
		//glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		//glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		depthCubeShader.use();
		for (int i = 0; i < 6; i++)
		{
			depthCubeShader.setUniform("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
			depthCubeShader.setUniform("far_plane", far);
			depthCubeShader.setUniform("lightPos", pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		}
		renderScene(depthCubeShader, cubePositions);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, WIDTH, HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

		glBindVertexArray(containerVAO);
		projection = glm::perspective(camera.Zoom, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		lightingShader.setUniform("projection", projection);
		lightingShader.setUniform("view", view);
		//lightingShader.setUniform("lightSpaceMatrix", lightProjection * lightView);
		lightingShader.setUniform("far_plane", far);
		renderScene(lightingShader, cubePositions);
		glBindVertexArray(0);

		//basicShader.use();
		//glBindVertexArray(quadVAO);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, depthMap);	// Use the color attachment texture as the texture of the quad plane
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//glBindVertexArray(0);

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		lampShader.setUniform("view", view);
		lampShader.setUniform("projection", projection);

		// Set matrices

		//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		//glStencilMask(0x00);	// disable writes
		//glDisable(GL_DEPTH_TEST);

		// draw upscaled cubes
		//glBindVertexArray(containerVAO);
		//for (int i = 0; i < 10; i++)
		//{
		//	model = glm::mat4();
		//	model = glm::translate(model, cubePositions[i]);
		//	GLfloat angle = 20.0f * i;
		//	model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
		//	model = glm::scale(model, glm::vec3(1.2f));
		//	lampShader.setUniform("model", model);
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//}
		//glBindVertexArray(0);

		//glStencilMask(0xFF);
		//glEnable(GL_DEPTH_TEST);
		//glDisable(GL_STENCIL_TEST);

		glBindVertexArray(lightVAO);
		glm::mat4 model;
		glBindTexture(GL_TEXTURE_2D, transparentTexture);
		for (int i = 0; i < 1; i++)
		{
			model = glm::mat4();
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			lampShader.setUniform("model", model);
			// Draw the light object (using light's vertex attributes)
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		//glBindVertexArray(transparentVAO);
		//lampShader.use();
		//for (int i = 0; i < vegetation.size(); i++)
		//{
		//	model = glm::mat4();
		//	model = glm::translate(model, vegetation[i]);
		//	lampShader.setUniform("model", model);
		//	glDrawArrays(GL_TRIANGLES, 0, 6);
		//}

		glBindVertexArray(0);
		//glEnable(GL_STENCIL_TEST);


		//view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		////view = camera.GetViewMatrix();
		//view = glm::rotate(view, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//projection = glm::perspective(camera.Zoom, (float)WIDTH / (float)HEIGHT, 0.1f, 10000.0f);

		//skyboxShader.use();
		//skyboxShader.setUniform("view", view);
		//skyboxShader.setUniform("projection", projection);
		//glBindVertexArray(skyboxVAO);
		//glActiveTexture(GL_TEXTURE0);
		//skyboxShader.setUniform("skybox", 0);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);

		glBindVertexArray(instancedVAO);
		glBindVertexArray(0);

		// Clear buffers
		//glClearColor(0.03f, 0.03f, 0.03f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//// Add transformation matrices
		//instancedShader.use();
		//glUniformMatrix4fv(glGetUniformLocation(instancedShader.getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

		//// Draw Planet
		//model = glm::mat4();
		//model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		//glUniformMatrix4fv(glGetUniformLocation(instancedShader.getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
		////planet.draw(instancedShader);

		//// Draw meteorites
		//glBindTexture(GL_TEXTURE_2D, rock.textures_loaded[0].id);
		//for (GLuint i = 0; i < rock.m_Meshes.size(); i++)
		//{
		//	glBindVertexArray(rock.m_Meshes[i].m_VAO);
		//	glDrawElementsInstanced(GL_TRIANGLES, rock.m_Meshes[i].m_Vertices.size(), GL_UNSIGNED_INT, 0, amount);
		//	glBindVertexArray(0);
		//	//glUniformMatrix4fv(glGetUniformLocation(instancedShader.getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(modelMatrices[i]));
		//	//rock.draw(instancedShader);
		//}

		//simpleShader.use();
		//view = camera.GetViewMatrix();
		////simpleShader.setUniform("view", view);
		////simpleShader.setUniform("projection", projection);
		//glBindVertexArray(simpleVAO);
		//glDrawArrays(GL_POINTS, 0, 4);
		//glBindVertexArray(0);


		//glEnable(GL_FRAMEBUFFER_SRGB);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//// Clear all relevant buffers
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		//glClear(GL_COLOR_BUFFER_BIT);
		//glDisable(GL_DEPTH_TEST); // We don't care about depth information when rendering a single quad

		//basicShader.use();
		//glBindVertexArray(quadVAO);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texColorBuffer);	// Use the color attachment texture as the texture of the quad plane
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//glBindVertexArray(0);

		// Swap the screen buffers
		glDisable(GL_FRAMEBUFFER_SRGB);
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void do_movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (keys[GLFW_KEY_SPACE])
		camera.ProcessKeyboard(UP, deltaTime);
	if (keys[GLFW_KEY_LEFT_CONTROL])
		camera.ProcessKeyboard(DOWN, deltaTime);
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

GLuint loadCubemap(std::vector<std::string> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);

	unsigned int width, height;
	GLubyte* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (int i = 0; i < faces.size(); i++)
	{
		image = loadImage(faces[i].c_str(), width, height);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		delete image;
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

void renderScene(Shader shader, glm::vec3 cubePositions[])
{
	glm::mat4 model;
	glBindVertexArray(planeVAO);
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0, -2, 0));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
	//model = glm::scale(model, glm::vec3(10, 10, 10));
	shader.setUniform("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Draw the container (using container's vertex attributes)
	glBindVertexArray(containerVAO);
	for (int i = 0; i < 10; i++)
	{
		model = glm::mat4();
		model = glm::translate(model, cubePositions[i]);
		GLfloat angle = 20.0f * i;
		model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
		shader.setUniform("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, -1.75f, 5.0f)); // Translate it down a bit so it's at the center of the scene
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
	glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	shader.setUniform("time", (float)glfwGetTime());
	//ourModel.draw(lightingShader);
	glBindVertexArray(0);
}

#endif