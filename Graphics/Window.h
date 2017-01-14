#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <string>
#include <FreeImage.h>
#include "..\Utils\Camera.h"

constexpr int MAX_KEYS = 1024;
constexpr int MAX_BUTTONS = 32;

class Window
{
public:
	static Window& Instance()
	{
		static Window instance;
		return instance;
	}

	Window(Window const&) = delete;
	void operator=(Window const&) = delete;

	void setParams(std::string name, int width, int height);
	
	void init();

	void update();
	bool shouldClose() const;
	void closeWindow();

	bool isKeyPressed(unsigned int keycode) const;
	bool isButtonPressed(unsigned int mousebutton) const;
	
	void moveCamera(const double& xpos, const double& ypos);

	inline int getWidth() const { return m_Width; }
	inline int getHeight() const { return m_Height; }
	inline int getInitWidth() const { return m_InitWidth; }
	inline int getInitHeight() const { return m_InitHeight; }
	inline float mouseX() const { return m_MousePosX; }
	inline float mouseY() const { return m_MousePosY; }
	inline bool isWindowResized() { return m_WindowResized; }
	inline void setWindowResized(bool flag) { m_WindowResized = flag; }
	inline GLFWwindow* getWindow() { return m_Window; }
	Camera& getCamera() { return m_Camera; }

private:
	Window();

	friend void window_resize(GLFWwindow* window, int width, int height);
	friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	friend void mouse_callback(GLFWwindow* window, double x, double y);

private:
	std::string m_Name;
	int m_InitWidth;
	int m_InitHeight;
	int m_Width;
	int m_Height;

	float m_MousePosX;
	float m_MousePosY;

	bool m_Keys[MAX_KEYS];
	bool m_MouseButtons[MAX_BUTTONS];

	GLFWwindow* m_Window;
	Camera m_Camera;

	bool m_WindowResized;
};