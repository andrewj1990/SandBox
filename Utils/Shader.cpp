#include "Shader.h"

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath)
{
	// 1. Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;
	// ensures ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::badbit);
	try
	{
		// Open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		if (geometryPath != nullptr)
		{
			gShaderFile.open(geometryPath);
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();
	// 2. Compile shaders
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];
	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// Print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// Print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// geometry shader
	GLuint geometry;
	if (geometryPath != nullptr)
	{
		const GLchar* gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(geometry, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}


	// Shader Program
	m_Program = glCreateProgram();
	glAttachShader(m_Program, vertex);
	glAttachShader(m_Program, fragment);
	if (geometryPath != nullptr) glAttachShader(m_Program, geometry);
	glLinkProgram(m_Program);
	// Print linking errors if any
	glGetProgramiv(m_Program, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(m_Program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geometry);

}

void Shader::use()
{
	glUseProgram(m_Program);
}

void Shader::unbind()
{
	glUseProgram(0);
}

void Shader::setUniform(const std::string& name, const glm::mat4& matrix)
{
	use();
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	//unbind();
}

void Shader::setUniform(const std::string& name, const glm::vec3& vector)
{
	use();
	glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(vector));
}

void Shader::setUniform(const std::string& name, const glm::vec2& vector)
{
	use();
	glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(vector));
}

void Shader::setUniform(const std::string& name, float x, float y, float z)
{
	use();
	glUniform3f(getUniformLocation(name), x, y, z);
}

void Shader::setUniform(const std::string& name, float value)
{
	use();
	glUniform1f(getUniformLocation(name), value);
}

void Shader::setUniform(const std::string& name, int value)
{
	use();
	glUniform1i(getUniformLocation(name), value);
}

void Shader::setUniform(const std::string& name, int count, int* value)
{
	use();
	glUniform1iv(getUniformLocation(name), count, value);	
}

GLuint Shader::getUniformLocation(const std::string& name)
{
	return glGetUniformLocation(m_Program, name.c_str());;
}
