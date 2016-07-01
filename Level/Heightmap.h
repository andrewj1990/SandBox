#pragma once

#include <GL\glew.h>
#include <vector>
#include <glm\glm.hpp>
#include "..\Utils\LoadImage.h"

class Heightmap
{
public:
	Heightmap();

	void loadMap();
	void render();

private:
	GLuint m_VAO;

	unsigned int m_Width;
	unsigned int m_Height;

};