#pragma once

#include <GL/glew.h>

class Buffer
{
public:
	Buffer(GLfloat* data, GLsizei count, GLuint componentCount);

	void bind() const;
	void unbind() const;
	GLuint getComponentCount() const;

private:
	GLuint m_BufferID;
	GLuint m_ComponentCount;

};
