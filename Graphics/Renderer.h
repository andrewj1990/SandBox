#pragma once

#include <cstddef>
#include <GL/glew.h>
#include "buffers/indexbuffer.h"
#include <vector>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "font.h"
#include <memory>

class Renderable;
class Entity;

struct VertexData
{
	glm::vec4 vertex;
	glm::vec2 uv;
	float tid;
	unsigned int colour;
};

const int RENDERER_MAX_SPRITES = 100000;
const int RENDERER_VERTEX_SIZE = sizeof(VertexData);
const int RENDERER_SPRITE_SIZE = RENDERER_VERTEX_SIZE * 4;
const int RENDERER_BUFFER_SIZE = RENDERER_SPRITE_SIZE * RENDERER_MAX_SPRITES;
const int RENDERER_INDICES_SIZE = RENDERER_MAX_SPRITES * 6;
constexpr int RENDERER_MAX_TEXTURES = 32;

constexpr int SHADER_VERTEX_INDEX = 0;
constexpr int SHADER_UV_INDEX = 1;
constexpr int SHADER_TID_INDEX = 2;
constexpr int SHADER_COLOUR_INDEX = 3;


class Renderer
{
private:
	GLuint m_VAO;
	GLuint m_BO;
	VertexData* m_Buffer;
	std::unique_ptr<IndexBuffer> m_IBO;
	GLsizei m_IndexCount;

	std::vector<GLuint> m_TextureSlots;

	std::vector<glm::mat4> m_TransformationStack;
	//std::unique_ptr<glm::mat4> m_TransformationBack;

public:
	Renderer();
	~Renderer();
	void begin();
	void submit(const Renderable& renderable);
	void drawString(const Font& font, const std::string& text, const glm::vec3& position, const glm::vec4& colour);
	void end();
	void flush();

	void render(const Renderable& renderable);
	void render(const std::vector<Renderable>& renderables);
	void render(const std::vector<std::unique_ptr<Renderable>>& renderables);
	void render(const std::vector<std::unique_ptr<Entity>>& entities);

	void push(const glm::mat4& matrix, bool override = false)
	{
		if (override)
		{
			m_TransformationStack.push_back(matrix);
		}
		else
		{
			m_TransformationStack.push_back(m_TransformationStack.back() * matrix);
		}

		//m_TransformationBack = std::make_unique<glm::mat4>(&m_TransformationStack.back());
	}

	void pop()
	{
		if (m_TransformationStack.size() > 1)
		{
			m_TransformationStack.pop_back();
		}

		//m_TransformationBack = std::make_unique<glm::mat4>(&m_TransformationStack.back());
	}

private:
	void init();

	glm::vec3 multiply(const glm::mat4& matrix, const glm::vec3 vector);

};

#include "renderable.h"
