#include "Settings.h"

void Settings::settingsToggle()
{
	valueToggle(GLFW_KEY_I, debugShowCollisionBoxes);
	valueToggle(GLFW_KEY_J, debugShowLightRange);
	valueToggle(GLFW_KEY_U, debugShowQuadTree);
	valueToggle(GLFW_KEY_O, noClip);
	valueToggle(GLFW_KEY_B, showLevelEditor);

}

Settings::Settings()
{
}

void Settings::valueToggle(unsigned int keyCode, bool& toggleVar)
{
	if (Window::Instance().isKeyTyped(keyCode))
	{
		std::cout << "toggled\n";
		toggleVar = !toggleVar;
	}
}
