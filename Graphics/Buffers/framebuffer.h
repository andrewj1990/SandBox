#pragma once

#include <GL\glew.h>
#include "..\Window.h"
#include "..\..\Utils\ResourceManager.h"
#include "..\texture.h"

class FrameBuffer
{
public:
	FrameBuffer(int width = 0, int height = 0);
	~FrameBuffer();

	void bind(bool clear = true);
	void unbind(bool clear = true);

	GLuint getTID() const { return m_Texture; }
	GLuint getVAO() const { return m_VAO; }
	Texture* getTexture() { return texture_; }

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
	GLuint m_RBO;

	Texture* texture_;
};