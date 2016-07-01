#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath = nullptr);

	void use();
	void unbind();
	void setUniform(const std::string& name, const glm::mat4& matrix);
	void setUniform(const std::string& name, const glm::vec3& vector);
	void setUniform(const std::string& name, const glm::vec2& vector);
	void setUniform(const std::string& name, float x, float y, float z);
	void setUniform(const std::string& name, float value);
	void setUniform(const std::string& name, int value);
	void setUniform(const std::string& name, int count, int* value);

	const GLuint& getProgram() const { return m_Program; }

private:
	GLuint getUniformLocation(const std::string& name);

private:
	GLuint m_Program;
};
