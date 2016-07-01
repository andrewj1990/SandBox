#pragma once

#include <GL/glew.h>

class IndexBuffer
{
private:
	GLuint m_BufferID;
	GLuint m_Count;

public:

	IndexBuffer(GLuint* data, GLsizei count);

	void bind() const;
	void unbind() const;
	GLuint getCount() const;
};
