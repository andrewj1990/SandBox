#pragma once

#define USE_DYNAMIC_STREAMING 0

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
class BoundingBox;
class Texture;

struct VertexData
{
	glm::vec4 vertex;
	glm::vec4 uv;
	float tid;
	unsigned int colour;
};

const int RENDERER_MAX_SPRITES	= 5000;
const int RENDERER_VERTEX_SIZE	= sizeof(VertexData);
const int RENDERER_SPRITE_SIZE	= RENDERER_VERTEX_SIZE * 4;
const int RENDERER_BUFFER_SIZE	= RENDERER_SPRITE_SIZE * RENDERER_MAX_SPRITES;
const int RENDERER_INDICES_SIZE = RENDERER_MAX_SPRITES * 6;

constexpr int RENDERER_MAX_TEXTURES = 32;
constexpr int SHADER_VERTEX_INDEX	= 0;
constexpr int SHADER_UV_INDEX		= 1;
constexpr int SHADER_TID_INDEX		= 2;
constexpr int SHADER_COLOUR_INDEX	= 3;


class Renderer
{
public:
	Renderer();
	~Renderer();

	void begin();
	void submit(const Renderable& renderable);
	void submit(const glm::vec3& position, const glm::vec2& size, const std::vector<glm::vec4>& uv, Texture* texture, const glm::vec4& colour = glm::vec4(1, 1, 1, 1));
	void drawString(const Font& font, const std::string& text, const glm::vec3& position, const glm::vec4& colour);
	void end();
	void flush();

	void render(const Renderable& renderable);
	void render(const std::vector<std::unique_ptr<Renderable>>& renderables);
	void render(const std::vector<std::shared_ptr<Renderable>>& renderables);

	void debugSubmit(const BoundingBox& bbox, Texture* texture = nullptr);
	void debugRender(const BoundingBox& bbox, Texture* texture = nullptr);
	
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
	}

	void pop()
	{
		if (m_TransformationStack.size() > 1)
		{
			m_TransformationStack.pop_back();
		}
	}

	GLenum m_SrcFactor = GL_SRC_ALPHA;
	GLenum m_BlendFactor = GL_ONE_MINUS_SRC_ALPHA;
	bool m_AlphaTest = true;

private:
	void init();

private:
	GLuint m_VAO;
	GLuint m_BO;
	VertexData* m_Buffer;
	VertexData* m_BufferInitPos;
	std::unique_ptr<IndexBuffer> m_IBO;
	GLsizei m_IndexCount;

	std::vector<GLuint> m_TextureSlots;
	std::vector<glm::mat4> m_TransformationStack;


};

#include "renderable.h"
