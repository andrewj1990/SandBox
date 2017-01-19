#include "Window.h"

Window::Window()
{
	for (int i = 0; i < MAX_KEYS; i++)
	{
		m_Keys[i] = m_KeyTyped[i] = m_KeyState[i] = false;
	}

	for (int i = 0; i < MAX_BUTTONS; i++)
	{
		m_MouseButtons[i] = m_MouseClicked[i] = m_MouseState[i] = false;
	}
}

void Window::setParams(std::string name, int width, int height)
{
	m_Name = name;
	m_Width = width;
	m_Height = height;
	m_InitWidth = width;
	m_InitHeight = height;
	m_Camera = Camera();
	m_WindowResized = false;

	init();
}

void Window::init()
{
	FreeImage_Initialise();

	if (!glfwInit())
	{
		std::cout << "glfw failed to initialize!\n";
		return;
	}

	m_Window = glfwCreateWindow(m_Width, m_Height, m_Name.c_str(), nullptr, nullptr);
	// fullscreen
	//GLFWmonitor* monitor = glfwGetPrimaryMonitor();

	//const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	//glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	//glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	//glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	//glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	//m_Window = glfwCreateWindow(m_Width, m_Height, m_Name.c_str(), monitor, nullptr);

	if (!m_Window)
	{
		glfwTerminate();
		return;
	}

	// set the required options 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	glfwMakeContextCurrent(m_Window);
	glfwSetWindowUserPointer(m_Window, this);

	glfwSetFramebufferSizeCallback(m_Window, window_resize);
	glfwSetKeyCallback(m_Window, key_callback);
	glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
	glfwSetCursorPosCallback(m_Window, mouse_callback);

	//glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// vsync
	glfwSwapInterval(0);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "glew failed to initialize\n";
		return;
	}

	std::cout << "OpenGL version : " << glGetString(GL_VERSION) << "\n";

	glViewport(0, 0, m_Width, m_Height);

}

void Window::update()
{
	// if key has been pressed and previous state was set to false then set key typed to true
	for (int i = 0; i < MAX_KEYS; i++)
	{
		m_KeyTyped[i] = m_Keys[i] && !m_KeyState[i];
	}

	for (int i = 0; i < MAX_BUTTONS; i++)
	{
		m_MouseClicked[i] = m_MouseButtons[i] && !m_MouseState[i];
	}

	// update key/button state
	memcpy(m_KeyState, m_Keys, MAX_KEYS);
	memcpy(m_MouseState, m_MouseButtons, MAX_BUTTONS);

	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}

bool Window::shouldClose() const
{
	return glfwWindowShouldClose(m_Window);
}

void Window::closeWindow()
{
	glfwSetWindowShouldClose(m_Window, true);
}

bool Window::isKeyPressed(unsigned int keycode) const
{
	return m_Keys[keycode];
}

bool Window::isKeyTyped(unsigned int keycode) const
{
	return m_KeyTyped[keycode];
}

bool Window::isButtonPressed(unsigned int mousebutton) const
{
	return m_MouseButtons[mousebutton];
}

bool Window::isButtonClicked(unsigned int mousebutton) const
{
	return m_MouseClicked[mousebutton];
}

void Window::moveCamera(const double& xpos, const double& ypos)
{
	m_Camera.lastPosX;
	if (m_Camera.firstRun)
	{
		m_Camera.lastPosX = xpos;
		m_Camera.lastPosY = ypos;
		m_Camera.firstRun = false;
	}

	GLfloat xoffset = xpos - m_Camera.lastPosX;
	GLfloat yoffset = m_Camera.lastPosY - ypos;  // Reversed since y-coordinates go from bottom to left

	m_Camera.lastPosX = xpos;
	m_Camera.lastPosY = ypos;

	m_Camera.ProcessMouseMovement(xoffset, yoffset);

}

float Window::getMouseWorldPosX() const
{
	//if (m_Camera.Zoom > 0)
	//{
	//}

	return m_Camera.Position.x + (mouseX() * (m_Camera.OrthoSize.x / m_Width)) + m_Camera.OrthoPos.x;
	//return m_Camera.Position.x + (mouseX() * (Settings::Instance().PROJECTION_WIDTH * (1.0f - m_Camera.Zoom)) / m_Width) + Settings::Instance().PROJECTION_WIDTH * (m_Camera.Zoom / 2.0f);
	//return m_Camera.Position.x + (mouseX() * Settings::Instance().PROJECTION_WIDTH / m_Width);
	//return m_Camera.Position.x + (mouseX() * (m_Camera.OrthoSize.x / m_Width));
}

float Window::getMouseWorldPosY() const
{
	//if (m_Camera.Zoom > 0)
	//{
	//}
	return m_Camera.Position.y + ((m_Height - mouseY()) * (m_Camera.OrthoSize.y / m_Height)) + m_Camera.OrthoPos.y;
	//return m_Camera.Position.y + ((m_Height - mouseY()) * ((Settings::Instance().PROJECTION_HEIGHT * (1.0f - m_Camera.Zoom)) / m_Height)) + Settings::Instance().PROJECTION_HEIGHT * (m_Camera.Zoom / 2.0f);
	//return m_Camera.Position.y + ((m_Height - mouseY()) * (Settings::Instance().PROJECTION_HEIGHT / m_Height));
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->m_Keys[key] = action != GLFW_RELEASE;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->m_MouseButtons[button] = action != GLFW_RELEASE;
}

void window_resize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->m_Width = width;
	win->m_Height = height;

	win->m_WindowResized = true;
}

void mouse_callback(GLFWwindow* window, double x, double y)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->m_MousePosX = x;
	win->m_MousePosY = y;
}