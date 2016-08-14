#pragma once

#include <GL\glew.h>
#include "..\Window.h"
#include "..\..\Utils\ResourceManager.h"

class FrameBuffer
{
public:
	FrameBuffer();

	void bind();
	void unbind();

	GLuint getTID() const { return m_Texture; }
	GLuint getVAO() const { return m_VAO; }

	void render();

private:
	// Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates
	const GLfloat quadVertices[24] = {
		// Positions   // TexCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};


private:
	GLuint m_FBO;
	GLuint m_Texture;
	GLuint m_VAO;
	GLuint m_VBO;

};