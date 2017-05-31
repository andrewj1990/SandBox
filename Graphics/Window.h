#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <string>
#include <FreeImage.h>

#include "..\Utils\Camera.h"
#include "..\Settings.h"

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
	bool isKeyTyped(unsigned int keycode) const;
	bool isButtonPressed(unsigned int mousebutton) const;
	bool isButtonReleased(unsigned int mousebutton) const;
	bool isButtonClicked(unsigned int mousebutton) const;
	void clearKeyPresses();
	void clearCharPress() { char_input_ = '\0'; }
	char getCharPressed() const { return (char)char_input_; }

	void moveCamera(const double& xpos, const double& ypos);

	float getMouseWorldPosX(bool zoom = true) const;
	float getMouseWorldPosY(bool zoom = true) const;

	inline int getWidth() const { return m_Width; }
	inline int getHeight() const { return m_Height; }
	inline int getInitWidth() const { return m_InitWidth; }
	inline int getInitHeight() const { return m_InitHeight; }
	inline float mouseX() const { return m_MousePosX < 0 ? 0 : m_MousePosX > m_Width ? m_Width : m_MousePosX; }
	inline float mouseY() const { return m_MousePosY < 0 ? 0 : m_MousePosY > m_Height ? m_Height : m_MousePosY; }
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
	friend void character_callback(GLFWwindow* window, unsigned int codepoint);

private:
	std::string m_Name;
	int m_InitWidth;
	int m_InitHeight;
	int m_Width;
	int m_Height;

	float m_MousePosX;
	float m_MousePosY;

	bool m_Keys[MAX_KEYS];
	bool m_KeyTyped[MAX_KEYS];
	bool m_KeyState[MAX_KEYS];
	bool m_MouseButtons[MAX_BUTTONS];
	bool m_MouseButtonsReleased[MAX_BUTTONS];
	bool m_MouseClicked[MAX_BUTTONS];
	bool m_MouseState[MAX_BUTTONS];

	GLFWwindow* m_Window;
	Camera m_Camera;

	bool m_WindowResized;

	unsigned int char_input_;
};