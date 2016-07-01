#include "Heightmap.h"

Heightmap::Heightmap()
{

}

void Heightmap::loadMap()
{
	GLubyte* image = loadImage("Textures/Heightmap.png", m_Width, m_Height);

	std::vector<std::vector<glm::vec3>> vertexCoords(m_Width, std::vector<glm::vec3>(m_Height));
	std::vector<std::vector<glm::vec2>> texCoords(m_Width, std::vector<glm::vec2>(m_Height));

	float texU = float(m_Width) * 0.1f;
	float texV = float(m_Height) * 0.1f;

	unsigned int ptr_inc = 3;
	unsigned int row_step = ptr_inc * m_Height;

	for (int row = 0; row < m_Height; row++)
	{
		for (int col = 0; col < m_Width; col++)
		{
			float rowStep = (float)row / (float)(m_Height - 1);
			float colStep = (float)col / (float)(m_Width - 1);
			float vertexHeight = (float)(*(image + row_step * row + col * ptr_inc) / 255.0f);
			vertexCoords[row][col] = glm::vec3(-0.5f + colStep, vertexHeight, -0.5f + rowStep);
			texCoords[row][col] = glm::vec2(texU * colStep, texV * rowStep);
		}
	}

}

void Heightmap::render()
{

}
